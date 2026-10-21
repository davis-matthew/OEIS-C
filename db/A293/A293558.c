/*
 * A293558  Triangle T(n,k) read by rows: T(n,k) =
 *          A005867(k-1)*A002110(n)/A002110(k).
 * https://oeis.org/A293558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Flattened triangle:  T(n,k) = ∏_{j=1}^{k-1} (p_j-1) · ∏_{j=k+1}^{n} p_j,
   with p_j the j‑th prime.  The sequence a(n) lists T(n,k) row by row,
   1‑based indexing. */

#include "../utils/sieve_primes.h"
#include <stddef.h>

/* static storage for the list of primes */
static idx_t *primes = NULL;
static size_t prime_cnt = 0;
static bool initialized = false;

/* initialise the prime table once */
static void init(void)
{
    if (initialized)
        return;

    /* limit fairly large: 2000 > 141st prime (827) needed for n≈10000 */
    idx_t limit = 2000;
    sieve_primes(limit, &primes, &prime_cnt);

    initialized = true;
}

/* a(n) for n >= 1 */
seq_t a(idx_t n)
{
    init();                      /* ensure primes are ready */

    /* find row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        r++;

    /* column k within that row */
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;           /* 1 <= k <= r */

    seq_t res = 1;

    /* multiply primes p1...(p_{k-1}) - 1 */
    for (idx_t j = 1; j < k; ++j)
        res *= (seq_t)(primes[j - 1] - 1);

    /* multiply primes p_{k+1}...p_r */
    for (idx_t j = k + 1; j <= r; ++j)
        res *= (seq_t)primes[j - 1];

    return res;
}
