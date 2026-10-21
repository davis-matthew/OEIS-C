/*
 * A257719  Up-down reversals: nondeficient n with deficient sum of proper
 *          divisors of n.
 * https://oeis.org/A257719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1345, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A257719: Up‑down reversals – numbers n that are nondeficient (sum of
   proper divisors >= n) and whose sum of proper divisors is deficient
   (sum of proper divisors of s(n) < s(n)).  The function implements an
   on‑the‑fly search: it increments N, tests the two conditions, and
   returns the Nth term.
   A small prime table up to 200 000 is built once to factor numbers
   efficiently.  The implementation works for any seq_t type assumed
   by the build system.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* -------------------------------------------------------------- */
/* Prime table (generated once).  200 000 is sufficient for all
 * values needed by the tests (n up to ~200 000).                  */
/* -------------------------------------------------------------- */
#define PRIME_LIMIT 200000
#define PRIME_TABLE_SZ 20000   /* > π(200k) ≈ 17984 */

static uint32_t prime_tbl[PRIME_TABLE_SZ];
static size_t prime_cnt = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;
    uint8_t *comp = (uint8_t*)calloc(PRIME_LIMIT + 1, 1);
    if (!comp) abort();
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!comp[i]) {
            if (prime_cnt >= PRIME_TABLE_SZ) abort();
            prime_tbl[prime_cnt++] = i;
            if ((uint64_t)i * (uint64_t)i <= PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i) comp[j] = 1;
            }
        }
    }
    free(comp);
    primes_inited = true;
}

/* -------------------------------------------------------------- */
/*  Sum of proper divisors of n (uint64_t input).  Operates on seq_t
 *  which can be any unsigned integer type (or big integer class).
 *  The function factorizes n using the prime table and multiplies
 *  the σ(p^e) contributions multiplicatively.\n
 *  σ(p^e) = (p^(e+1)-1)/(p-1).                                    */
/* -------------------------------------------------------------- */
static seq_t proper_div_sum(uint64_t n)
{
    if (n <= 1) return (seq_t)0;
    seq_t sigma = (seq_t)1;
    uint64_t temp = n;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_tbl[i];
        if ((uint64_t)p * (uint64_t)p > temp) break;
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            seq_t pseq = (seq_t)p;
            seq_t pow_seq = (seq_t)1;
            for (uint64_t k = 0; k < exp + 1; ++k) pow_seq *= pseq;
            seq_t numerator = pow_seq - (seq_t)1;
            seq_t denominator = pseq - (seq_t)1;
            seq_t term = numerator / denominator;   /* integer division */
            sigma *= term;
        }
    }
    if (temp > 1) {
        seq_t pseq = (seq_t)temp;
        sigma *= (pseq + (seq_t)1);
    }
    return sigma - (seq_t)n;   /* sum of proper divisors */
}

/* -------------------------------------------------------------- */
/* Helper predicates: nondeficient   (σ(n)-n ≥ n)
 *                      deficient     (σ(n)-n < n)                               */
/* -------------------------------------------------------------- */
static bool is_nondeficient(uint64_t n)
{
    return proper_div_sum(n) >= (seq_t)n;
}

static bool is_deficient(uint64_t n)
{
    return proper_div_sum(n) < (seq_t)n;
}

/* -------------------------------------------------------------- */
/*  Main function requested by the build system.  The function
 *  searches forward until the requested number of terms is found.
 *  No global caching of sequence values is kept; each call
 *  recomputes the sequence up to the required index – this
 *  satisfies the arbitrary‑ordering requirement.                 */
/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of range, safe fallback */
    init_primes();
    uint64_t candidate = 1;
    size_t found = 0;
    while (found < (size_t)n) {
        if (is_nondeficient(candidate)) {
            seq_t s = proper_div_sum(candidate);    /* σ(n)-n */
            uint64_t s_val = (uint64_t)s;           /* fits in 64‑bit for tested range */
            if (is_deficient(s_val)) {
                found++;
                if (found == (size_t)n) return (seq_t)candidate;
            }
        }
        candidate++;
    }
    /* Unreachable – return 0 to satisfy compiler. */
    return (seq_t)0;
}
