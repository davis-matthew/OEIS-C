/*
 * A331163  a(n) is the number of occurrences of the most frequently seen
 *          difference between adjacent digits in the concatenation of a(0) to
 *          a(n-1), with a(0) = 0, a(1) = 0.
 * https://oeis.org/A331163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the number of occurrences of the most frequently seen
 *            difference between adjacent digits in the concatenation of a(0)
 *            … a(n‑1), with a(0)=0, a(1)=0
 * Range:     u8 n <= 785, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static state: memoised sequence and counters */
static seq_t *a_cache     = NULL;   /* array of sequence values */
static size_t a_len       = 0;      /* number of values stored */
static size_t a_cap       = 0;      /* capacity of a_cache */
static bool   initialized = false;  /* init flag */

/* difference counters and last processed digit */
static unsigned long long diff_cnt[10] = {0};
static int last_digit = -1;         /* -1 means no previous digit */

/* Ensure a_cache has at least 'needed' capacity. */
static void ensure_cache_cap(size_t needed)
{
    if (a_cap < needed) {
        size_t new_cap = a_cap * 2 + 1;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cache = tmp;
        a_cap   = new_cap;
    }
}

/* Process all digits of a_cache[term_idx], updating diff_cnt and last_digit. */
static void process_term(idx_t term_idx)
{
    seq_t val = a_cache[term_idx];
    /* For extraction of decimal digits from arbitrary precision types,
       use repeated division by 10.  Digits are stored least‑significant
       first, then processed from most‑significant to least. */
    uint8_t digits[128];
    size_t len = 0;

    if (val == (seq_t)0) {
        digits[0] = 0;
        len       = 1;
    } else {
        seq_t tmp = val;
        while (tmp > (seq_t)0) {
            seq_t r   = tmp % (seq_t)10;
            digits[len++] = (uint8_t)r;
            tmp = tmp / (seq_t)10;
        }
    }

    int prev = last_digit;
    for (size_t k = len; k > 0; --k) {
        uint8_t d = digits[k - 1];
        if (prev >= 0) {
            unsigned diff = (prev > d) ? (prev - d) : (d - prev);
            diff_cnt[diff]++;               /* counts fit in 64 bit for n≈10⁴ */
        }
        prev = d;
    }
    last_digit = prev;   /* last digit of this term */
}

/* Return the maximum count among diff_cnt[0…9]. */
static unsigned long long max_diff_count(void)
{
    unsigned long long mx = 0;
    for (int i = 0; i < 10; ++i) if (diff_cnt[i] > mx) mx = diff_cnt[i];
    return mx;
}

/* One–time initialization: store a(0) and process its digits. */
static void init(void)
{
    if (initialized) return;

    ensure_cache_cap(1);
    a_cache[0] = (seq_t)0;
    a_len      = 1;

    /* reset counters and process a(0) */
    for (int i = 0; i < 10; ++i) diff_cnt[i] = 0;
    last_digit = -1;
    process_term(0);

    initialized = true;
}

/* External sequence function */
seq_t a(idx_t n)
{
    init();

    if ((size_t)n < a_len) return a_cache[n];

    ensure_cache_cap((size_t)n + 1);

    for (size_t i = a_len; i <= (size_t)n; ++i) {
        a_cache[i] = (seq_t)max_diff_count(); /* a(i) */
        process_term(i);                     /* update counts for next term */
    }
    a_len = (size_t)n + 1;

    return a_cache[n];
}
