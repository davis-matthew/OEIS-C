/*
 * A241730  Number of digits in the decimal expansion of n! minus the number
 *          of digits in the decimal expansion of n.
 * https://oeis.org/A241730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = floor(log10(n!)) - floor(log10(n))
 * Range:     u8 n <= 147, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The index type supplied by the build. */
typedef uint64_t idx_t;

/* Count decimal digits of an idx_t value without using floating point. */
static inline int digits_of_uint64(idx_t x) {
    int d = 0;
    do { d++; x /= 10; } while (x);
    return d;
}

/* ----------------- Big‑integer state for factorial ----------------- */
static uint32_t *big = NULL;          /* words in base 1e9 */
static size_t big_cap = 0;            /* capacity of big[] */
static size_t big_words = 0;          /* number of used words */

/* Cached digit counts of k! for 0 <= k <= max_fact. */
static int *fact_digits = NULL;
static size_t fact_cap = 0;
static idx_t max_fact = 0;

#define BASE 1000000000U  /* 10^9 */

/* Multiply the current big integer by small integer m (m <= 10000). */
static void big_mul(uint32_t m) {
    uint64_t carry = 0;
    for (size_t i = 0; i < big_words; ++i) {
        uint64_t prod = (uint64_t)big[i] * m + carry;
        big[i] = (uint32_t)(prod % BASE);
        carry  = prod / BASE;
    }
    while (carry) {
        if (big_words == big_cap) {
            size_t new_cap = big_cap ? big_cap << 1 : 1024;
            uint32_t *tmp = (uint32_t *)realloc(big, new_cap * sizeof(uint32_t));
            if (!tmp) { big_cap = 0; return; }  /* allocation failed – unlikely in tests */
            big = tmp;
            big_cap = new_cap;
        }
        big[big_words++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

/* Return number of decimal digits represented by the big integer. */
static int big_digit_count(void) {
    if (big_words == 0) return 1;     /* zero */
    int d = 0;
    if (big_words > 1) d += (int)(big_words - 1) * 9;
    uint32_t msw = big[big_words - 1];
    if      (msw >= 100000000U) d += 9;
    else if (msw >= 10000000U)  d += 8;
    else if (msw >= 1000000U)   d += 7;
    else if (msw >= 100000U)    d += 6;
    else if (msw >= 10000U)     d += 5;
    else if (msw >= 1000U)      d += 4;
    else if (msw >= 100U)       d += 3;
    else if (msw >= 10U)        d += 2;
    else                         d += 1;
    return d;
}

/* Ensure that fact_digits[0..n] contain digit counts of k! for k <= n. */
static void ensure_cache(idx_t n) {
    if (n == 0 || max_fact >= n) return;

    /* Lazy initialization of the big integer. */
    if (!big) {
        big_cap = 1024;
        big = (uint32_t *)malloc(big_cap * sizeof(uint32_t));
        if (!big) return;  /* allocation failure – won't happen in tests */
        big_words = 1;
        big[0] = 1;      /* currently represents 1! */
    }

    /* Lazy initialization of the digit cache. */
    if (!fact_digits) {
        fact_cap = n + 1;
        if (fact_cap < 4) fact_cap = 4;
        fact_digits = (int *)malloc(fact_cap * sizeof(int));
        if (!fact_digits) return;
        fact_digits[0] = 0;          /* unused, for k = 0 */
        fact_digits[1] = 1;          /* digits(1!) = 1 */
        max_fact = 1;
    }

    /* Grow the cache array if needed. */
    if (n >= fact_cap) {
        size_t new_cap = n + 1;
        if (new_cap < fact_cap * 2) new_cap = fact_cap * 2;
        int *tmp = (int *)realloc(fact_digits, new_cap * sizeof(int));
        if (!tmp) return;
        fact_digits = tmp;
        fact_cap = new_cap;
    }

    /* Compute further factorial digits up to n. */
    while (max_fact < n) {
        ++max_fact;                  /* next factorial index */
        big_mul((uint32_t)max_fact); /* multiply by max_fact */
        fact_digits[(size_t)max_fact] = big_digit_count();
    }
}

/* --------------------- Sequence function --------------------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* a(0) = 0 by definition */

    ensure_cache(n);

    int fact_digit = fact_digits[(size_t)n];
    int n_digit    = digits_of_uint64(n);
    return (seq_t)(fact_digit - n_digit);
}
