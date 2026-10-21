/*
 * A287058  Sum of decimal digits of 118^n.
 * https://oeis.org/A287058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digit_sum(118^n)
 * Range:     u8 n <= 25, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

typedef struct {
    uint8_t *digits;   /* least‑significant decimal digit first */
    idx_t   len;       /* number of used digits */
    size_t  capacity;  /* allocated size of the array */
} BigInt;

/* Current value of 118^k for the highest computed index */
static BigInt pow118 = {NULL, 0, 0};

/* Cached digit sums: sum_cache[i] = a(i) for i <= cache_n */
static seq_t *sum_cache = NULL;
static idx_t cache_n = 0;  /* highest index that has been computed */
static size_t sum_cap = 0; /* current capacity of sum_cache */

/* Ensure that sum_cache can hold the entry for index n */
static void ensure_sum_cache(idx_t n)
{
    while (sum_cap <= (size_t)n) {
        sum_cap = (sum_cap > 0) ? sum_cap * 2 : 1;
        sum_cache = (seq_t *)realloc(sum_cache, sum_cap * sizeof(seq_t));
    }
}

/* Ensure that pow118 has enough room for needed digits */
static void ensure_pow_cap(size_t needed)
{
    while (pow118.capacity < needed) {
        pow118.capacity = (pow118.capacity > 0) ? pow118.capacity * 2 : 8;
        pow118.digits = (uint8_t *)realloc(pow118.digits,
                                           pow118.capacity * sizeof(uint8_t));
    }
}

/* Multiply the current power by 118 */
static void multiply_pow118(void)
{
    uint32_t carry = 0;
    for (idx_t i = 0; i < pow118.len; ++i) {
        uint32_t prod = (uint32_t)pow118.digits[i] * 118u + carry;
        pow118.digits[i] = (uint8_t)(prod % 10u);
        carry = prod / 10u;
    }
    while (carry > 0) {
        if ((size_t)pow118.len >= pow118.capacity) {
            ensure_pow_cap((size_t)pow118.len + 1);
        }
        pow118.digits[pow118.len++] = (uint8_t)(carry % 10u);
        carry /= 10u;
    }
}

/* Compute the sum of decimal digits of pow118 */
static seq_t digit_sum(void)
{
    seq_t s = 0;
    for (idx_t i = 0; i < pow118.len; ++i) {
        s += pow118.digits[i];
    }
    return s;
}

/* Lazy initialization of structures */
static void init(void)
{
    if (pow118.digits == NULL) {
        pow118.capacity = 8;
        pow118.digits = (uint8_t *)malloc(pow118.capacity * sizeof(uint8_t));
        pow118.digits[0] = 1;          /* 118^0 = 1 */
        pow118.len = 1;

        sum_cap = 1;
        sum_cache = (seq_t *)malloc(sum_cap * sizeof(seq_t));
        sum_cache[0] = 1;              /* digit sum of 1 */
        cache_n = 0;
    }
}

/* Public function: returns a(n) */
seq_t a(idx_t n)
{
    init();                                 /* set up once */

    if (n > cache_n) {                      /* compute up to n */
        for (idx_t i = cache_n + 1; i <= n; ++i) {
            multiply_pow118();             /* pow118 = 118^i */
            seq_t s = digit_sum();          /* sum of its digits */
            ensure_sum_cache(i);
            sum_cache[i] = s;
        }
        cache_n = n;
    }
    return sum_cache[n];
}
