/*
 * A342945  Numbers m such that d(1)^1 + d(2)^2 + ... + d(p)^k = d(1)! + d(2)!
 *          + ... + d(k)!, where d(i), i=1..k, are the digits of m.
 * https://oeis.org/A342945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   factorial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th element of OEIS A342945:
   the smallest integer m whose digits d(1)…d(k) satisfy
   Σ d(i)^i == Σ d(i)!  (i = 1..k)  where d(1) is the leftmost digit. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factorial.h"
#include "../utils/seq_pow.h"

static seq_t *cache        = NULL;   /* stored sequence terms          */
static size_t cache_len   = 0;       /* number of terms in cache       */
static size_t cache_cap   = 0;       /* allocated cache capacity       */
static seq_t next_to_check = 1;       /* next integer to test           */
static seq_t fact_digit[10];         /* factorial values for 0..9      */
static bool  fact_init    = false;   /* whether fact_digit initialized */

/*----- initialise the factorial table for digits 0..9 -------------------*/
static void init_fact_digits(void)
{
    if (!fact_init) {
        for (int d = 0; d <= 9; ++d)
            fact_digit[d] = factorial((idx_t)d);
        fact_init = true;
    }
}

/*----- test whether a candidate m satisfies the equality ----------------*/
static bool is_valid_term(seq_t m)
{
    if (m == 0) return false;     /* exclude 0, not in the sequence     */

    seq_t tmp = m;
    unsigned char rev[32];        /* maximum 32 decimal digits on 64‑bit int */
    size_t len = 0;

    /* extract digits in reverse order */
    while (tmp > 0) {
        unsigned char digit = (unsigned char)(tmp % 10);
        rev[len++] = digit;
        tmp /= 10;
    }

    seq_t sum_pow = 0;
    seq_t sum_fac = 0;

    /* compute the two sums */
    for (size_t i = 0; i < len; ++i) {
        unsigned char digit = rev[i];
        idx_t pos = (idx_t)(len - i);   /* positions start at 1 from the left */

        seq_t pow_val = seq_pow((seq_t)digit, pos);
        seq_t fac_val = fact_digit[digit];

        sum_pow += pow_val;
        sum_fac += fac_val;
    }

    return sum_pow == sum_fac;
}

/*----- public sequence function ---------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset < 1 */

    init_fact_digits();

    /* generate terms until we have at least n in the cache */
    while (cache_len < (size_t)n) {
        seq_t m = next_to_check;
        next_to_check++;           /* advance candidate for next call */

        if (is_valid_term(m)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
                seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                /* assume realloc succeeds; no error handling needed for tests */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = m;
        }
    }

    return cache[n - 1];
}
