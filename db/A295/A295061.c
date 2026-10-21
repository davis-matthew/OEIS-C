/*
 * A295061  Solution of the complementary equation a(n) = 4*a(n-2) + b(n-1),
 *          where a(0) = 1, a(1) = 3, b(0) = 2, and (a(n)) and (b(n)) are
 *          increasing complementary sequences.
 * https://oeis.org/A295061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A295061:  a(n) = 4*a(n-2) + b(n-1), 
 * a(0)=1, a(1)=3, b(0)=2.
 * a() and b() are increasing complementary sequences.  */
#include <stdlib.h>
#include <stdbool.h>

/* static cache */
static seq_t *a_cache = NULL;   /* stores a(0)...a(k)   */
static seq_t *b_cache = NULL;   /* stores b(0)...b(k-1) */
static size_t a_len  = 0;        /* number of a values stored */
static size_t b_len  = 0;        /* number of b values stored */
static size_t cache_cap = 0;     /* capacity of the arrays        */
static seq_t  last_b_val = 0;    /* last computed b value         */
static idx_t  ptr_a = 0;         /* scanning pointer for a()      */
static idx_t  ptr_b = 0;         /* scanning pointer for b()      */

static void ensure_capacity(void)
{
    if (a_len >= cache_cap) {
        cache_cap = (cache_cap == 0) ? 8 : cache_cap << 1;
        a_cache = (seq_t *)realloc(a_cache, cache_cap * sizeof(seq_t));
        b_cache = (seq_t *)realloc(b_cache, cache_cap * sizeof(seq_t));
    }
}

/* Compute the next unused integer (next b value) */
static seq_t next_b_value(void)
{
    seq_t candidate = last_b_val + 1;    /* first integer after the last b */
    while (true) {
        /* Skip a values that are smaller than the candidate */
        while (ptr_a < (idx_t)a_len && a_cache[ptr_a] < candidate)
            ++ptr_a;
        /* Skip b values that are smaller than the candidate */
        while (ptr_b < (idx_t)b_len && b_cache[ptr_b] < candidate)
            ++ptr_b;

        bool used = false;
        if (ptr_a < (idx_t)a_len && a_cache[ptr_a] == candidate) {
            used = true;
            ++ptr_a;
        }
        if (ptr_b < (idx_t)b_len && b_cache[ptr_b] == candidate) {
            used = true;
            ++ptr_b;
        }
        if (!used)  /* candidate is unused */
            break;
        ++candidate; /* try next integer */
    }
    return candidate;
}

/* Main function required by the exercise */
seq_t a(idx_t n)
{
    /* Initialise the cache on first call */
    if (a_cache == NULL) {
        cache_cap = 8;
        a_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        b_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)3;
        a_len = 2;          /* a(0), a(1) stored */

        b_cache[0] = (seq_t)2;
        b_cache[1] = (seq_t)4;
        b_len = 2;          /* b(0), b(1) stored */
        last_b_val = b_cache[1];  /* 4 */

        ptr_a = 0;
        ptr_b = 0;
    }

    if (n < (idx_t)a_len)    /* already computed */
        return a_cache[n];

    /* Compute values up to a(n) */
    size_t target = (size_t)n;
    for (size_t i = a_len; i <= target; ++i) {
        /* Ensure b(i-1) exists */
        while (b_len <= i-1) {
            seq_t val = next_b_value();
            ensure_capacity();
            b_cache[b_len] = val;
            ++b_len;
            last_b_val = val;
        }

        /* Compute a(i) = 4*a(i-2) + b(i-1) */
        seq_t a_val = (seq_t)4 * a_cache[i-2] + b_cache[i-1];
        ensure_capacity();
        a_cache[a_len] = a_val;
        ++a_len;
    }

    return a_cache[n];
}
