/*
 * A295573  Array read by upwards antidiagonals: T(n,k) = nk + floor(phi n)
 *          ceiling(phi k) where phi = (1 + sqrt(5))/2.
 * https://oeis.org/A295573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where T(i,j) = i*j + ⌊φ⋅i⌋·⌈φ⋅j⌉ (i,j ≥ 1) φ =
 *            (1+√5)/2. The sequence is obtained by reading the array T(i,j)
 *            along upward antidiagonals (bottom‑to‑top)
 * Range:     u8 n <= 126, u16 n <= 9906, u32 n <= 9906, u64 n <= 9906,
 *            u128 n <= 9906, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Pre‑compute Fibonacci numbers needed for Beatty floor(φ·m). */
static bool fib_init = false;
static unsigned long long fib[94];        /* fib[0] = 0, fib[1] = 1,
                                           fib[93] < 2⁶⁴, fits in uint64_t */
static void init_fib(void)
{
    if (fib_init) return;
    fib[0] = 0;
    fib[1] = 1;
    for (size_t i = 2; i < 94; ++i)
        fib[i] = fib[i - 1] + fib[i - 2];
    fib_init = true;
}

/* floor(φ·m) using a convergent F_{k+1}/F_k.  For all m with
   F_k > m, floor(m·φ) = floor(m·F_{k+1}/F_k). */
static seq_t floor_phi(idx_t m)
{
    init_fib();
    size_t k = 1;
    while (fib[k] <= (unsigned long long)m) ++k;   /* ensure fib[k] > m */

    /* fib[k+1] is defined for all practical m (our m < fib[93]). */
    idx_t num = (idx_t)m * fib[k + 1];
    idx_t den = fib[k];
    idx_t div = num / den;
    return (seq_t)div;
}

/* Convert a linear index n (≥1) into the coordinates (i,j)
   of the array T(i,j) when read along upward antidiagonals. */
static void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    /* Find diagonal number s so that (s‑1)*s/2 ≥ n.  s starts at 2. */
    idx_t s = 2;
    while ((s - 1) * s / 2 < n) ++s;

    idx_t elems_before = (s - 2) * (s - 1) / 2;
    idx_t offset = n - elems_before - 1;          /* 0‑based offset in diag */

    *i = (s - 1) - offset;      /* bottom element has i = s‑1, j = 1 */
    *j = offset + 1;
}

/* Main sequence entry point. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n<1, safe fallback */

    idx_t i, j;
    antidiagonal_coords(n, &i, &j);

    seq_t floor_phi_i = floor_phi(i);          /* ⌊φ·i⌋ */
    seq_t floor_phi_j = floor_phi(j);          /* ⌊φ·j⌋ */
    seq_t ceil_phi_j  = floor_phi_j + 1;       /* ⌈φ·j⌉ = ⌊φ·j⌋ + 1 */

    seq_t ii = (seq_t)i;
    seq_t jj = (seq_t)j;

    return ii * jj + floor_phi_i * ceil_phi_j;
}
