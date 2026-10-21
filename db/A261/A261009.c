/*
 * A261009  Write 2^n in base 3, add up the "digits".
 * https://oeis.org/A261009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of the base‑3 digits of 2^n (A261009)
 * Range:     u8 n <= 388, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *sum_cache   = NULL; /* cached a(k) values      */
static size_t   cache_cap = 0;    /* allocated size of cache */

static uint8_t *digits     = NULL;   /* base‑3 digits of 2^current_n, LSB first */
static size_t   digits_len = 0;      /* used length of digits          */
static size_t   digits_cap = 0;      /* allocated capacity of digits   */

static idx_t current_n = 0;          /* n for which digits represent 2^n */

/* Ensure the digit array can hold at least 'need' digits. */
static void ensure_digits(size_t need)
{
    if (digits_cap >= need) return;
    size_t new_cap = digits_cap ? digits_cap * 2U : 2U;
    while (new_cap < need) new_cap <<= 1;
    uint8_t *p = (uint8_t *)realloc(digits, new_cap * sizeof(uint8_t));
    if (!p) abort();
    digits = p;
    digits_cap = new_cap;
}

/* Ensure the cache can store values up to index 'need' (inclusive). */
static void ensure_cache(size_t need)
{
    if (cache_cap > need) return;
    size_t new_cap = cache_cap ? cache_cap * 2U : 2U;
    while (new_cap < need) new_cap <<= 1;
    seq_t *p = (seq_t *)realloc(sum_cache, new_cap * sizeof(seq_t));
    if (!p) abort();
    sum_cache = p;
    cache_cap = new_cap;
}

/* One‑time initialization of the data structures. */
static void init(void)
{
    if (!sum_cache) {
        sum_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!sum_cache) abort();
        cache_cap = 1;
        sum_cache[0] = (seq_t)1;      /* a(0) = sum of digits of 1 = 1   */
    }
    if (!digits) {
        digits = (uint8_t *)malloc(sizeof(uint8_t));
        if (!digits) abort();
        digits_cap = 1;
        digits_len = 1;
        digits[0]   = 1;              /* 2^0 in base 3 is "1"              */
    }
    current_n = 0;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) { init(); initialized = 1; }

    /* Return cached value if already computed. */
    if ((size_t)n <= current_n) return sum_cache[(size_t)n];

    /* Expand cache and digit array as necessary. */
    ensure_cache((size_t)n + 1);
    ensure_digits((size_t)n + 1);  /* an upper bound on digit count */

    /* Compute values up to n. */
    while (current_n < n) {
        /* current sum of digits for 2^current_n */
        seq_t sum = sum_cache[(size_t)current_n];
        unsigned int carry = 0;
        size_t i;

        /* Multiply the base‑3 number by 2 in place. */
        for (i = 0; i < digits_len; ++i) {
            unsigned int old = digits[i];
            unsigned int prod = old * 2U + carry;     /* 0 ≤ prod ≤ 6 */
            unsigned int newd = prod % 3U;
            carry = prod / 3U;
            digits[i] = (uint8_t)newd;

            /* Update the digit sum safely, avoiding unsigned underflow. */
            if (newd >= old)
                sum += (seq_t)(newd - old);
            else
                sum -= (seq_t)(old - newd);
        }

        /* Append remaining carry as new digits if necessary. */
        while (carry) {
            unsigned int newd = carry % 3U;
            carry         /= 3U;
            if (digits_len == digits_cap)
                ensure_digits(digits_len + 1);
            digits[digits_len++] = (uint8_t)newd;
            sum += (seq_t)newd;
        }

        /* Store the newly computed sum and advance. */
        sum_cache[(size_t)(++current_n)] = sum;
    }

    return sum_cache[(size_t)n];
}
