/*
 * A356661  a(n) = n! * Sum_{d|n} 1/d^(n/d - 1).
 * https://oeis.org/A356661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{d|n} 1 / d^(n/d - 1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   free_seq_array, mul_small, seq_alloc_new_array, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/mul_small.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* ----- factorial cache ----- */
    static seq_t *fact_cache = NULL;
    static idx_t fact_cache_size = 0;   /* number of factorials stored */
    static idx_t fact_cache_cap = 0;    /* allocated capacity (>= size) */

    /* initialise cache if needed */
    if (fact_cache == NULL) {
        fact_cache_cap = n + 1;
        fact_cache     = seq_alloc_new_array(fact_cache_cap);
        fact_cache[0]  = 1;   /* 0! = 1 */
        fact_cache_size = 1;
    }
    /* grow cache if required */
    if (n >= fact_cache_cap) {
        size_t new_cap = fact_cache_cap ? fact_cache_cap << 1 : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_arr = seq_alloc_new_array(new_cap);
        for (idx_t i = 0; i < fact_cache_size; ++i)
            new_arr[i] = fact_cache[i];
        free_seq_array(fact_cache);
        fact_cache = new_arr;
        fact_cache_cap = new_cap;
    }
    /* compute missing factorials up to n */
    while (fact_cache_size <= n) {
        seq_t prev = fact_cache[fact_cache_size - 1];
        seq_t next = mul_small(prev, (unsigned int)fact_cache_size);
        fact_cache[fact_cache_size] = next;
        ++fact_cache_size;
    }

    seq_t fact_n = fact_cache[n];

    /* ----- enumerate divisors of n ----- */
    enum { MAX_DIVS = 128 };   /* safe for n <= 10000 */
    idx_t divs[MAX_DIVS];
    size_t div_count = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            divs[div_count++] = d;
            if (d != n / d && div_count < MAX_DIVS)
                divs[div_count++] = n / d;
        }
    }

    /* ----- accumulate sum ----- */
    seq_t sum = 0;
    for (size_t i = 0; i < div_count; ++i) {
        idx_t d   = divs[i];
        idx_t exp = n / d - 1;
        seq_t pow = seq_pow((seq_t)d, exp);
        seq_t term = fact_n / pow;
        sum += term;
    }

    return sum;
}
