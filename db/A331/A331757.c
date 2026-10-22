/*
 * A331757  Number of edges in a figure made up of a row of n adjacent
 *          congruent rectangles upon drawing diagonals of all possible
 *          rectangles.
 * https://oeis.org/A331757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 8 for n > 1: a(n) = 2 * ( n*(n+3) + Σ_{i=2}^{⌊n/2⌋}
 *            (n+1-i)*(n+1+i)*φ(i) + Σ_{i=⌊n/2⌋+1}^{n} (n+1-i)*(2*n+2-i)*φ(i)
 *            ) where φ(i) is Euler's totient function
 * Range:     u8 n <= 4, u16 n <= 21, u32 n <= 355, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache of φ(k) values – we recompute from scratch whenever
   more entries are needed. */
static idx_t *phi_cache = NULL;   /* phi_cache[0] unused */
static size_t phi_cap   = 0;

/* Recompute Euler's totient up to ‘n’.  The array is resized to
   hold 0..n (index 0 unused).  Realloc is used; the pointer is
   cast explicitly for C++ acceptance. */
static void ensure_phi(idx_t n)
{
    if (n <= (idx_t)phi_cap)      /* already have sufficient entries */
        return;

    idx_t *new_phi = (idx_t *)realloc(phi_cache, (size_t)(n + 1) * sizeof(idx_t));
    if (!new_phi) {
        /* fatal: out of memory */
        exit(EXIT_FAILURE);
    }
    phi_cache = new_phi;

    /* Initialise: φ(k)=k (will be corrected below) */
    for (idx_t k = 0; k <= n; ++k)
        phi_cache[k] = k;
    if (n >= 1)
        phi_cache[1] = 1;               /* φ(1)=1 */

    /* Standard sieve for φ: for each prime p subtract φ[j]/p from φ[j] */
    for (idx_t p = 2; p <= n; ++p) {
        if (phi_cache[p] == p) {        /* p is prime */
            for (idx_t j = p; j <= n; j += p) {
                phi_cache[j] -= phi_cache[j] / p;
            }
        }
    }

    phi_cap = (size_t)n;               /* remember highest computed index */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1)          /* base case given in OEIS entry */
        return (seq_t)8;

    ensure_phi(n);

    seq_t N = (seq_t)n;                /* promote index to seq_t */
    seq_t base = N * (N + (seq_t)3);   /* n*(n+3) part of the formula */
    seq_t sum = (seq_t)0;

    idx_t mid = n / 2;                  /* floor(n/2) */

    /* first sum: i = 2 .. floor(n/2) */
    for (idx_t i = 2; i <= mid; ++i) {
        seq_t ii      = (seq_t)i;
        seq_t term1   = (N + (seq_t)1 - ii) * (N + (seq_t)1 + ii);
        seq_t phi_i   = (seq_t)phi_cache[i];
        sum += term1 * phi_i;
    }

    /* second sum: i = floor(n/2)+1 .. n */
    for (idx_t i = mid + 1; i <= n; ++i) {
        seq_t ii      = (seq_t)i;
        seq_t term1   = (N + (seq_t)1 - ii) * ((seq_t)2 * N + (seq_t)2 - ii);
        seq_t phi_i   = (seq_t)phi_cache[i];
        sum += term1 * phi_i;
    }

    return (seq_t)2 * (base + sum);
}
