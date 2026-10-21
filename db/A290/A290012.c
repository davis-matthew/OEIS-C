/*
 * A290012  a(n) is the smallest prime number p satisfying p^2 >= Sum_{1 <= k
 *          <= n} prime(k)^2.
 * https://oeis.org/A290012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 646, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A290012: a(n) = smallest prime p with
   p^2 ≥ Σ_{k=1..n} prime(k)^2. */

/*--- Standard headers --------------------------------------------------*/
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/*--- Private helper tables ---------------------------------------------*/
static uint64_t *primes      = NULL; /* primes[i] = (i+1)-th prime          */
static size_t   primes_len   = 0;    /* number of primes stored             */
static size_t   primes_cap   = 0;    /* allocated capacity                   */

static seq_t   *sumSquares   = NULL; /* cumulative sums of square of primes */
static size_t   sum_len      = 0;    /* equal to primes_len                  */
static size_t   sum_cap      = 0;    /* allocated capacity                   */

/*--- Grow the arrays to hold at least 'new_cap' elements. --------------*/
static void grow_arrays(size_t new_cap)
{
    if (new_cap <= primes_cap)
        return;

    /* grow primes */
    uint64_t *pnew = NULL;
#ifdef __cplusplus
    pnew = reinterpret_cast<uint64_t*>(
        realloc(primes, new_cap * sizeof(uint64_t)));
#else
    pnew = (uint64_t*)realloc(primes, new_cap * sizeof(uint64_t));
#endif
    if (!pnew)
        exit(1);
    primes     = pnew;
    primes_cap = new_cap;

    /* grow sumSquares */
    seq_t *snew = NULL;
#ifdef __cplusplus
    snew = reinterpret_cast<seq_t*>(
        realloc(sumSquares, new_cap * sizeof(seq_t)));
#else
    snew = (seq_t*)realloc(sumSquares, new_cap * sizeof(seq_t));
#endif
    if (!snew)
        exit(1);
    sumSquares = snew;
    sum_cap    = new_cap;
}

/*--- Test primality using the primes already stored -------------------*/
static bool is_prime_candidate(uint64_t cand)
{
    for (size_t j = 0; j < primes_len; ++j) {
        const uint64_t p = primes[j];
        if (p * p > cand)
            break;
        if (cand % p == 0)
            return false;
    }
    return true;
}

/*--- Generate the next prime and append it to the tables. -------------*/
static void generate_next_prime(void)
{
    uint64_t cand;
    if (primes_len == 0) {
        cand = 2;
    } else {
        cand = primes[primes_len - 1] + 1;
        if (cand > 2 && (cand & 1) == 0)
            ++cand;         /* keep odd after 2 */
    }

    for (;;) {
        if (is_prime_candidate(cand)) {
            grow_arrays(primes_len + 1);
            primes[primes_len] = cand;

            seq_t sq = (seq_t)cand * (seq_t)cand;
            if (primes_len == 0)
                sumSquares[primes_len] = sq;
            else
                sumSquares[primes_len] = sumSquares[primes_len - 1] + sq;

            ++primes_len;
            ++sum_len;
            return;
        }
        cand += 2;         /* skip even numbers after 2 */
    }
}

/*--- Ensure that at least 'needed' primes are in the tables. ----------*/
static void ensure_primes(size_t needed)
{
    while (primes_len < needed)
        generate_next_prime();
}

/*--- Main sequence function -------------------------------------------*/
seq_t a(idx_t n)
{
    /* n must be ≥ 1; we ignore values < 1 per the specification. */
    if (n == 0)
        return 0;

    /* First compute the sum of squares of the first n primes. */
    ensure_primes((size_t)n);
    const seq_t sum = sumSquares[(size_t)(n - 1)];

    /* Then find the smallest prime p with p^2 ≥ sum. */
    while ((seq_t)primes[primes_len - 1] * (seq_t)primes[primes_len - 1] < sum)
        generate_next_prime();

    /* Binary search in the primes array. */
    size_t lo = 0, hi = primes_len - 1;
    while (lo < hi) {
        const size_t mid = lo + ((hi - lo) >> 1);
        const seq_t psq = (seq_t)primes[mid] * (seq_t)primes[mid];
        if (psq < sum)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (seq_t)primes[lo];
}
