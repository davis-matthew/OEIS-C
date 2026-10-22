/*
 * A332597  Number of edges in a "frame" of size n X n (see Comments in
 *          A331776 for definition).
 * https://oeis.org/A332597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(1) = 8 * for n > 1: * a(n) = 4*(n-1)*(8*n-1) * + 8 *
 *            Σ_{i=2}^{⌊n/2⌋} (n+1-i)*(n+i+1)*φ(i) * + 8 * Σ_{i=⌊n/2⌋+1}^{n}
 *            (n+1-i)*(2*n+2-i)*φ(i) * * φ(i) is Euler's totient function. The
 *            function keeps a static array * of φ values which is extended on
 *            demand. Summation indices are * handled with 64...
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 251, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stdint.h>   /* uint64_t */

/* ---- static storage for the φ table ---- */
static uint64_t *phi_cache = NULL;   /* φ values up to phi_max */
static idx_t    phi_max   = 0;       /* largest computed index */

/* ---- ensure φ values are available up to n ---- */
static void _ensure_phi(idx_t n)
{
    if (n <= phi_max)
        return;          /* already computed */

    /* grow the array to size n+1 (indices 0…n) */
    idx_t new_len = n + 1;
    uint64_t *tmp = (uint64_t *)realloc(phi_cache,
                                         new_len * sizeof(uint64_t));
    if (!tmp)
        abort();          /* out of memory */
    phi_cache = tmp;

    /* initialise each entry to its index; φ(0) is set to 0 */
    for (idx_t i = 0; i < new_len; ++i)
        phi_cache[i] = (uint64_t)i;

    /* sieve for φ */
    for (idx_t p = 2; p < new_len; ++p) {
        if (phi_cache[p] == (uint64_t)p) {            /* p is prime */
            for (idx_t j = p; j < new_len; j += p) {
                phi_cache[j] = phi_cache[j] / (uint64_t)p
                                * ((uint64_t)p - 1);
            }
        }
    }

    phi_max = n;
}

/* ---- the requested sequence function ---- */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)8;         /* offset 1 */

    /* make sure φ values up to n are computed */
    _ensure_phi(n);

    /* cast n to seq_t for safe arithmetic */
    const seq_t N  = (seq_t)n;
    const seq_t N1 = N + 1;       /* n + 1 */

    /* base term: 4*(n-1)*(8*n-1) */
    const seq_t base = (seq_t)4 * (N - 1) * ((seq_t)8 * N - 1);

    /* sum over i = 2 .. floor(n/2) */
    const idx_t half = n / 2;
    seq_t sum = (seq_t)0;

    for (idx_t i = 2; i <= half; ++i) {
        const seq_t ii = (seq_t)i;
        const seq_t a  = N1 - ii;           /* n + 1 - i */
        const seq_t b  = N + ii + 1;        /* n + i + 1 */
        const seq_t term = a * b *
                           (seq_t)phi_cache[i];
        sum += term;
    }

    /* sum over i = floor(n/2)+1 .. n */
    for (idx_t i = half + 1; i <= n; ++i) {
        const seq_t ii = (seq_t)i;
        const seq_t a  = N1 - ii;                 /* n + 1 - i */
        const seq_t b  = (seq_t)2 * N + 2 - ii;   /* 2*n + 2 - i */
        const seq_t term = a * b *
                           (seq_t)phi_cache[i];
        sum += term;
    }

    /* combine all parts */
    const seq_t result = base + (seq_t)8 * sum;
    return result;
}
