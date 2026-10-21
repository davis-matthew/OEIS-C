/*
 * A262871  Sum of the squarefree numbers appearing among the smaller parts of
 *          the partitions of n into two parts.
 * https://oeis.org/A262871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..⌊n/2⌋} i * μ(i)^2 . i.e. sum of the squarefree
 *            numbers up to ⌊n/2⌋
 * Range:     u8 n <= 59, u16 n <= 925, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*--------------------------------------------------------------
 *  Incrementally compute the Möbius values and the prefix sums
 *  of the squarefree numbers.  The implementation recomputes the
 *  data from scratch every time the maximum limit increases;
 *  this keeps the logic simple and eliminates the subtle bug
 *  caused by partially resharing the sieve state.
 *-------------------------------------------------------------
 */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* static state used for incremental computation */
static idx_t            pre_max   = 0;     /* largest n processed  */
static int8_t          *mu        = NULL;  /* Möbius value 0,±1   */
static unsigned char   *isComp    = NULL;  /* composite flag     */
static seq_t           *prfx      = NULL;  /* prefix sums        */
static uint32_t        *primes    = NULL;  /* list of primes     */
static size_t          prime_cnt = 0;      /* number of primes   */
static size_t          prime_cap = 0;      /* prime array capacity */

/* Ensure the primes array can hold at least `need` elements */
static void primes_ensure(size_t need)
{
    if (need <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 64;
    if (new_cap < need) new_cap = need;
    uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
    if (!tmp) abort();   /* out of memory */
    primes = tmp;
    prime_cap = new_cap;
}

/* Extend all precomputed arrays up to `limit` (inclusive).
 * The sieve is recomputed from scratch each time `limit`
 * increases.  This approach guarantees correctness and
 * simplicity for the required range n ≤ 10000. */
static void extend_to(idx_t limit)
{
    if (limit <= pre_max) return;

    size_t sz = (size_t)(limit + 1);
    mu     = (int8_t *)realloc(mu,      sz * sizeof(int8_t));
    isComp = (unsigned char *)realloc(isComp, sz * sizeof(unsigned char));
    prfx   = (seq_t *)realloc(prfx,    sz * sizeof(seq_t));
    if (!mu || !isComp || !prfx) abort();

    /* initialise arrays for new entries */
    mu[0] = 0; mu[1] = 1;
    isComp[0] = 0; isComp[1] = 0;
    for (size_t i = 2; i <= limit; ++i) {
        mu[i] = 1;           /* provisional value */
        isComp[i] = 0;
    }

    /* reset prime list for this recomputation */
    prime_cnt = 0;
    primes_ensure((size_t)limit + 1);

    /* linear sieve to compute Möbius values */
    for (idx_t i = 2; i <= limit; ++i) {
        if (!isComp[i]) {          /* i is prime */
            primes[prime_cnt++] = (uint32_t)i;
            mu[i] = -1;
        }
        for (size_t k = 0; k < prime_cnt; ++k) {
            idx_t p = primes[k];
            idx_t t = i * p;
            if (t > limit) break;
            isComp[t] = 1;
            if (i % p == 0) {
                mu[t] = 0;
                break;
            } else {
                mu[t] = (int8_t)(-mu[i]);
            }
        }
    }

    /* compute prefix sums of squarefree numbers */
    prfx[0] = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        prfx[i] = prfx[i - 1] + ((mu[i] != 0) ? (seq_t)i : (seq_t)0);
    }

    pre_max = limit;
}

/*--------------------------------------------------------------
 *  Public function for the OEIS sequence A262871
 *-------------------------------------------------------------
 */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* ⌊n/2⌋ is 0 for n=1 */
    idx_t limit = n / 2;           /* upper bound of the summation */
    if (limit == 0) return 0;
    extend_to(limit);
    return prfx[limit];
}
