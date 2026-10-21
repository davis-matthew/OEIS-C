/*
 * A281001  Square array read by antidiagonals downwards: A(n, 1) = smallest
 *          Wieferich prime to base n and A(n, k) = smallest Wieferich prime
 *          to base A(n, k-1) for k > 1.
 * https://oeis.org/A281001
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 15, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   build_primes_upto, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A281001: a(n) is the square-array read by antidiagonals.
   A(n,1) = smallest Wieferich prime to base n,
   A(n,k) = smallest Wieferich prime to base A(n,k-1), k>1.
   a(n) is obtained by reading the array diagonally downwards
   starting from (2,1).  The offset is 2. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Helper functions from the lib */
#include "../utils/mod_pow.h"              /* mod_pow(seq_t base, uint64_t exp, seq_t mod) */
#include "../utils/build_primes_upto.h"    /* build_primes_upto(idx_t limit,
                                              uint64_t **primes_out,
                                              size_t *cnt_out) */

/* ---------- Prime sieve ----------
   We build a list of primes up to a fixed limit.  The limit is chosen
   large enough to contain the prime 4000000 which is much larger than
   any prime that can appear in a Wieferich test for the ranges we
   evaluate.                                   */
static uint64_t *prime_list = NULL;
static size_t   prime_cnt  = 0;
static bool     primes_inited = false;
static void init_primes(void)
{
    if (primes_inited) return;
    const idx_t LIMIT = 4000000;            /* 4 000 000 > all primes needed   */
    build_primes_upto(LIMIT, &prime_list, &prime_cnt);
    primes_inited = true;
}

/* ---------- Utility: primality test ----------
   Uses the pre‑generated prime list and simple trial division.   */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_cnt &&
         (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= x; ++i)
    {
        if (x % prime_list[i] == 0) return false;
    }
    return true;
}

/* ---------- Find the smallest Wieferich prime to a given base ----------
   A prime p is Wieferich to base b iff b^(p-1) ≡ 1 (mod p^2).        */
static seq_t smallest_wieferich(seq_t base)
{
    init_primes();

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = prime_list[i];
        seq_t    mod = (seq_t)p * (seq_t)p;                /* p²  */
        seq_t    res = mod_pow(base % mod, p-1, mod);       /* b^(p-1) mod p² */
        if (res == (seq_t)1) return (seq_t)p;
    }

    /* If not found in the precomputed range (highly unlikely for our
       application), extend the search with odd candidates.       */
    uint64_t cand = prime_list[prime_cnt-1] | 1U;           /* start with odd > last prime */
    for (;; cand += 2U)
    {
        if (!is_prime_uint64(cand)) continue;
        seq_t mod = (seq_t)cand * (seq_t)cand;
        seq_t res = mod_pow(base % mod, cand-1, mod);
        if (res == (seq_t)1) return (seq_t)cand;
    }
}

/* ---------- Main sequence function ----------
   The mapping from index n to the pair (i,j) in the array is computed
   directly from the antidiagonal traversal.                          */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                    /* undefined below offset */

    /* Zero‑based position in the antidiagonal order */
    idx_t pos = n - 2;

    /* Find diagonal number d such that d*(d+1)/2 > pos */
    idx_t d = 1;
    while ((unsigned __int128)d * (d + 1) / 2 <= pos) ++d;

    idx_t prev = (unsigned __int128)(d - 1) * d / 2;
    idx_t offset = pos - prev;               /* 0‑based inside this diagonal */

    /* Row and column (1‑based) in the array */
    uint64_t i = offset + 2;                 /* rows start at 2   */
    uint64_t j = d - offset;                 /* columns start at 1 */

    /* Compute A(i,j) iteratively */
    seq_t val = (seq_t)i;
    for (uint64_t step = 1; step <= j; ++step)
        val = smallest_wieferich(val);

    return val;
}
