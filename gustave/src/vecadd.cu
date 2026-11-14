#include "check.cuh"

__global__ void vecadd_kernel(const float* a, const float* b, float* c, int n)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n)
    {
        c[i] = a[i] + b[i];
    }
}

int main()
{
    constexpr int n = 54;
    float a[n], b[n], c[n] = {};
    for (int i = 0; i < n; ++i)
    {
        a[i] = i;
        b[i] = 2 * i;
    }

    float *d_a, *d_b, *d_c;
    check(cudaMalloc(&d_a, sizeof(a)));
    check(cudaMalloc(&d_b, sizeof(b)));
    check(cudaMalloc(&d_c, sizeof(c)));
    check(cudaMemcpy(d_a, a, sizeof(a), cudaMemcpyHostToDevice));
    check(cudaMemcpy(d_b, b, sizeof(b), cudaMemcpyHostToDevice));
    vecadd_kernel<<<(n + 31) / 32, 32>>>(d_a, d_b, d_c, n);
    check(cudaGetLastError());
    check(cudaDeviceSynchronize());
    check(cudaMemcpy(c, d_c, sizeof(c), cudaMemcpyDeviceToHost));
    check(cudaFree(d_a));
    check(cudaFree(d_b));
    check(cudaFree(d_c));

    for (int i = 0; i < n; ++i)
    {
        if (c[i] != a[i] + b[i])
        {
            std::fprintf(stderr, "vecadd: mismatch at %d\n", i);
            return 1;
        }
    }
    std::printf("vecadd: all %d results correct (last = %.0f)\n", n, c[n - 1]);
}
