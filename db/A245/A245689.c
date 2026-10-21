/*
 * A245689  Smallest divisor of n that is greater than the smallest prime not
 *          dividing n (A053669(n)).
 * https://oeis.org/A245689
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest divisor of n that is greater than the smallest
 *            prime not dividing n. The function works for any seq_t and idx_t
 *            supplied by the build
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------------------------------------------------------------------- */
/*  The implementation relies on the utility functions `build_primes`
   (to build a table of primes) and `factor_u64` (to factorise an
   unsigned 64‑bit integer).  All other predicates are standard
   C/C++ headers.  ----------------------------------------- */
/* -------------------------------------------------------------------- */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/build_primes.h"   /* void build_primes(idx_t limit,
                                      idx_t **out, size_t *cnt); */
#include "../utils/factor_u64.h"     /* void factor_u64(uint64_t n,
                                      uint64_t *pr, uint64_t *ex, size_t *cnt); */

/* -------------------------------------------------------------------- */
/*  Hold the prime table:  static for all calls of a().                */
/* -------------------------------------------------------------------- */
static uint64_t *prime_list = NULL;   /* pointer to array of primes     */
static size_t    prime_cnt   = 0;     /* number of primes in the table  */

/* -------------------------------------------------------------------- */
/*  Build the prime table (once) up to a fixed upper limit.  Many
   applications of this sequence use n <= 10 000, hence primes up to
   200 000 are more than enough.  ------------------------------------- */
/* -------------------------------------------------------------------- */
static void ensure_prime_table(void)
{
    if (prime_list != NULL)          /* already built   */
        return;

    const idx_t LIMIT = 200000;      /* 2*10^5 is safe   */

    build_primes(LIMIT, &prime_list, &prime_cnt);
    /* For the limits we care about the table should always exist.   */
    if (prime_list == NULL || prime_cnt == 0) {
        /* In an unlikely error case we abort – the function will
           simply return 0 for all n.  */
        prime_list = NULL;
        prime_cnt  = 0;
    }
}

/* -------------------------------------------------------------------- */
/*  Helper: for a given prime factorisation (pr[], ex[], fcnt) the
   function recursively visits every divisor of the original number.
   The smallest divisor that is strictly greater than `p_not` is kept
   in *best.  ----------------------------------------------- */
/* -------------------------------------------------------------------- */
static void gen_divisors(const uint64_t *pr,            /* prime bases   */
                         const uint64_t *ex,            /* corresponding exponents   */
                         size_t fcnt,                   /* how many distinct primes   */
                         size_t idx,                    /* current prime index       */
                         uint64_t current,              /* divisor under construction */
                         uint64_t p_not,                /* minimal divisor threshold  */
                         uint64_t *best)                /* result                 */
{
    if (idx == fcnt) {
        if (current > p_not && current < *best)
            *best = current;
        return;
    }
    uint64_t mult = 1;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        gen_divisors(pr, ex, fcnt, idx + 1,
                     current * mult, p_not, best);
        mult *= pr[idx];
    }
}

/* -------------------------------------------------------------------- */
/*  Find the smallest prime not dividing n.  -------------------------------- */
/* -------------------------------------------------------------------- */
static uint64_t smallest_prime_not_dividing(uint64_t n)
{
    /* First try primes from the precomputed table.                 */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (n % p != 0)
            return p;
    }

    /* If n is divisible by all primes up to the table limit,
       search for the next prime beyond the last entry.  This
       situation never occurs for n <= 10 000; for safety we
       still handle it.                                 */
    uint64_t candidate = prime_list[prime_cnt - 1] + 1;
    for (;;) {
        bool prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > candidate)
                break;
            if (candidate % p == 0) {
                prime = false;
                break;
            }
        }
        if (prime && candidate % n != 0)  /* must not divide n */
            return candidate;
        ++candidate;
    }
}

/* -------------------------------------------------------------------- */
/*  Main sequence function.  ------------------------------------------- */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;              /* undefined for n < 3 */

    ensure_prime_table();            /* ensure primes are ready */

    const uint64_t N = (uint64_t)n;

    /* step 1: smallest prime not dividing n */
    uint64_t p_not = smallest_prime_not_dividing(N);

    /* step 2: factor n (unsigned 64-bit) */
    uint64_t primes_table[64];
    uint64_t exps_table[64];
    size_t   fcnt = 0;
    factor_u64(N, primes_table, exps_table, &fcnt);

    /* step 3: enumerate divisors and keep the smallest > p_not   */
    uint64_t best = N + 1;          /* any divisor cannot exceed N   */
    gen_divisors(primes_table, exps_table,
                 fcnt, 0, 1ULL, p_not, &best);

    return (seq_t)best;              /* cast to the generic seq_t type */
}
