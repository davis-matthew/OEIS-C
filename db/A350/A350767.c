/*
 * A350767  a(1)=1. Thereafter, a(n+1) is the least unused number k such that
 *          either d(j(n)) properly divides d(k) or d(k) properly divides
 *          d(j(n)), where j(n) = a(n)+1 and d is the divisor counting
 *          function A000005.
 * https://oeis.org/A350767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   this function implements OEIS A350767 Recurrence: a(1) = 1 For n
 *            >= 1, j(n) = a(n)+1 d(j) = divisor_count(j) a(n+1) = least
 *            unused integer k such that d(k) properly divides d(j) or d(j)
 *            properly divides d(k) This implementation memoises the sequence
 *            and the prime table and computes divisor counts...
 * Range:     u8 n <= 212, u16 n <= 9370, u32 n <= 9370, u64 n <= 9370,
 *            u128 n <= 9370, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* --------------------------------------------------------------------- */
/* Prime sieve helpers (basic, no special headers)                      */
/* --------------------------------------------------------------------- */

static uint64_t *prime_list = NULL;
static size_t   prime_count = 0;
static uint64_t prime_limit = 0;

static void
ensure_primes(uint64_t limit)
{
    if (limit <= prime_limit)
        return;

    bool *is_composite = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_composite) exit(1);

    for (uint64_t p = 2; p * p <= limit; ++p)
        if (!is_composite[p])
            for (uint64_t m = p * p; m <= limit; m += p)
                is_composite[m] = true;

    uint64_t *new_list = NULL;
    size_t    new_cnt  = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) {
            new_list = (uint64_t *)realloc(new_list,
                      (new_cnt + 1) * sizeof(uint64_t));
            new_list[new_cnt++] = i;
        }

    free(is_composite);
    free(prime_list);
    prime_list = new_list;
    prime_count = new_cnt;
    prime_limit = limit;
}

/* --------------------------------------------------------------------- */
/* Divisor counting by factorisation using the prime sieve                */
/* --------------------------------------------------------------------- */

static seq_t
divisor_count_u64(uint64_t n)
{
    if (n == 0)
        return 0;

    seq_t res = 1;
    uint64_t tmp = n;

    /* ensure primes up to sqrt(tmp) */
    uint64_t limit = (uint64_t)(tmp > 1 ? tmp : 1);
    /* rough upper bound to not allocate too big */
    if (limit > 1000000)
        limit = 1000000;
    ensure_primes(limit);

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p > tmp / p)        /* p > sqrt(tmp) */
            break;
        if (tmp % p == 0) {
            seq_t exp = 0;
            do {
                tmp /= p;
                ++exp;
            } while (tmp % p == 0);
            res *= (exp + 1);
        }
    }
    if (tmp > 1)                /* remaining prime factor */
        res *= 2;
    return res;
}

/* --------------------------------------------------------------------- */
/* Used integer tracker (boolean array)                                 */
/* --------------------------------------------------------------------- */

static bool *used = NULL;
static size_t used_cap = 0;

static void
ensure_used_capacity(uint64_t idx)
{
    if (idx < used_cap)
        return;
    size_t new_cap = used_cap ? used_cap * 2 : 2;
    while (new_cap <= idx)
        new_cap <<= 1;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!tmp) exit(1);
    memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used = tmp;
    used_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Main sequence routine                                                  */
/* --------------------------------------------------------------------- */

seq_t
a(idx_t n)
{
    if (n == 0)                /* undefined, but avoid UB */
        return 0;

    static idx_t  cur_n  = 0;   /* how many terms have been stored (1-based) */
    static seq_t *seq    = NULL;
    static size_t seq_cap = 0;

    /* initialise the used array once */
    if (!used_cap) {
        used_cap = 1;
        used = (bool *)calloc(used_cap, sizeof(bool));
        if (!used) exit(1);
    }

    while (cur_n < n) {
        if (cur_n == 0) {      /* a(1) = 1 */
            if (!seq_cap) {
                seq = (seq_t *)malloc(sizeof(seq_t));
                seq_cap = 1;
                if (!seq) exit(1);
            } else
                seq = (seq_t *)realloc(seq, sizeof(seq_t));
            seq[0] = 1;
            ensure_used_capacity(1);
            used[1] = true;
            ++cur_n;
            continue;
        }

        /* compute next term */
        uint64_t prev      = (uint64_t)seq[cur_n - 1];
        uint64_t j_val     = prev + 1;
        seq_t   d_j        = divisor_count_u64(j_val);

        uint64_t candidate = 1;
        for (;;) {
            if (candidate >= used_cap)
                ensure_used_capacity(candidate);

            if (used[candidate]) {
                ++candidate;
                continue;
            }

            seq_t d_c = divisor_count_u64(candidate);

            if ( (d_c % d_j == 0 && d_c / d_j > 1) ||
                 (d_j % d_c == 0 && d_j / d_c > 1) ) {

                if (cur_n == seq_cap) {
                    seq_cap = seq_cap ? seq_cap * 2 : 4;
                    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                    if (!seq) exit(1);
                }
                seq[cur_n] = candidate;
                used[candidate] = true;
                ++cur_n;
                break;
            }
            ++candidate;
        }
    }

    return seq[n - 1];
}
