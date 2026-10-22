/*
 * A333838  a(n) is the greatest integer q <= n such that for some r >= q,
 *          phi(q) + phi(r) = 2*n.
 * https://oeis.org/A333838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: For each n ≥ 1, a(n) is the greatest integer q ≤ n for which there
 * exists an integer r ≥ q such that φ(q) + φ(r) = 2·n.  
 * This implementation pre‑computes Euler’s totient φ(m) for 1 ≤ m ≤ LIMIT
 * (LIMIT = 200 000 suffices for all test cases up to n = 10 000).  
 * For every totient value k it records the largest m with φ(m) = k.
 * Then, given n, it scans q from n down to 1 and checks whether a suitable
 * r exists by verifying that maxR[2*n‑φ(q)] ≥ q.  If no such q is found,
 * the function returns 0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Build supplies typedefs for seq_t and idx_t. */
static const idx_t LIMIT = 200000;        /* pre‑compute φ up to LIMIT */

static bool initialized = false;
static uint64_t *phi_arr = NULL;    /* φ(m) for 0 ≤ m ≤ LIMIT */
static idx_t    *maxR    = NULL;    /* largest m with φ(m)=k for 0 ≤ k ≤ LIMIT */

/* One‑time precomputation of φ and the maxR table. */
static void precompute(void)
{
    if (initialized)
        return;
    initialized = true;

    /* Allocate arrays.  Cast the result of calloc for C++. */
    phi_arr = (uint64_t*)calloc(LIMIT + 1, sizeof(uint64_t));
    maxR    = (idx_t*)calloc(LIMIT + 1,   sizeof(idx_t));
    if (!phi_arr || !maxR)
        abort();

    /* Initialise φ(m) = m. */
    for (idx_t i = 0; i <= LIMIT; ++i)
        phi_arr[i] = i;

    /* Euler sieve for φ. */
    for (idx_t p = 2; p <= LIMIT; ++p)
    {
        if (phi_arr[p] == p)                /* p is prime */
        {
            for (idx_t j = p; j <= LIMIT; j += p)
                phi_arr[j] -= phi_arr[j] / p;
        }
    }

    /* Build maxR table: for each k = φ(m), store the largest m. */
    for (idx_t m = 1; m <= LIMIT; ++m)
    {
        uint64_t k = phi_arr[m];
        if (m > maxR[k])
            maxR[k] = m;
    }
}

/* Required sequence function. */
seq_t a(idx_t n)
{
    /* Pre‑compute once. */
    precompute();

    if (n == 0)
        return (seq_t)0;

    idx_t twoN = 2 * n;   /* 2·n fits in idx_t for n ≤ 10 000 */

    for (idx_t q = n; q >= 1; --q)
    {
        uint64_t phiq = phi_arr[q];                  /* φ(q) */
        if (phiq > twoN)                            /* impossible target */
            continue;

        idx_t target = twoN - phiq;                 /* φ(r) needed */

        /* target will be ≤ 2*n ≤ 20 000, so also ≤ LIMIT. */
        if (maxR[target] >= q)                      /* r ≥ q exists */
            return (seq_t)q;
    }

    /* No suitable q found. */
    return (seq_t)0;
}
