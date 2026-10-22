/*
 * A347533  Array A(n,k) where A(n,0) = n and A(n,k) = (k*n + 1)^2 - A(n,k-1),
 *          n > 0, read by ascending antidiagonals.
 * https://oeis.org/A347533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = A(n,k) where (n,k) is the pair obtained by reading the *
 *            2‑D array A(n,k) by ascending antidiagonals. The array is
 *            defined by * * A(n,0) = n * A(n,k) = (k*n + 1)^2 - A(n,k-1) for
 *            k > 0 * * The entries can also be computed directly: * * Let
 *            T(k) = k*(k+1)/2 (the k‑th triangular number) * *...
 * Range:     u8 n <= 40, u16 n <= 719, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Multiply two 64 bit numbers using 128‑bit intermediate to avoid overflow
 * while searching for the antidiagonal.  The product will fit in
 * unsigned __int128 on all supported platforms. */
static inline __uint128_t mul128(uint64_t a, uint64_t b)
{
    return (__uint128_t)a * b;
}

/* Find the smallest d such that d*(d+1)/2 >= m.
 * m is 1‑based index into the antidiagonal sequence.
 */
static uint64_t find_diagonal(uint64_t m)
{
    uint64_t lo = 1, hi = m;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        /* (mid*(mid+1))/2 >= m ?  */
        if (mul128(mid, mid + 1) / 2 >= m) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}

/* Main sequence function required by the build system. */
seq_t a(idx_t n)
{
    /* The OEIS offset is 1, but indices n in the caller are 1‑based. */
    uint64_t m = n;                  /* 1‑based position in the antidiagonal read */
    uint64_t d = find_diagonal(m);   /* diagonal number (1‑based) */
    /* Number of terms before the current diagonal: d*(d-1)/2 */
    uint64_t before = mul128(d, d - 1) / 2;
    uint64_t pos = m - before;       /* position within diagonal (1‑based) */
    uint64_t k = pos - 1;            /* column index, 0‑based */
    uint64_t row = d - pos + 1;      /* n index (1‑based)  */
    /* Convert to seq_t for calculation. */
    seq_t nseq = (seq_t)row;
    seq_t kseq = (seq_t)k;
    seq_t tri = kseq * (kseq + 1) / 2;   /* triangular number T(k) */

    if (k & 1U) { /* k is odd */
        return tri * nseq * nseq + kseq * nseq + 1;
    } else {      /* k is even */
        return tri * nseq * nseq + (kseq + 1) * nseq;
    }
}
