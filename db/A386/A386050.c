/*
 * A386050  Primes having only {0, 2, 5, 9} as digits.
 * https://oeis.org/A386050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 34, u32 n <= 42, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A386050: primes whose decimal digits are only 0, 2, 5, or 9 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;          /* dynamically grown array of valid primes */
static size_t cached = 0;            /* number of terms stored in cache      */
static size_t cap = 0;               /* current allocated capacity of cache  */

static struct generator {
    size_t len;             /* current length (number of digits)    */
    uint8_t idx[64];        /* indices into allowed digits per pos  */
} gen;

/* allowed digit lists */
static const seq_t first_digits[3] = { (seq_t)2, (seq_t)5, (seq_t)9 };
static const seq_t all_digits[4]   = { (seq_t)0, (seq_t)2, (seq_t)5, (seq_t)9 };

/* increment the generator to the next number with allowed digits */
static void gen_next(void)
{
    int pos = (int)gen.len - 1;
    while (pos >= 0) {
        if (pos == 0) {                          /* most significant digit */
            if (gen.idx[0] < 2) {
                gen.idx[0]++;                    /* 2 -> 5 -> 9 -> overflow */
                return;
            } else {                             /* overflow -> increase length */
                for (size_t i = 0; i < gen.len; ++i)
                    gen.idx[i] = 0;               /* reset all to 0 */
                gen.len++;
                return;
            }
        } else {                                 /* other positions (4 digits) */
            if (gen.idx[pos] < 3) {
                gen.idx[pos]++;                    /* 0 -> 2 -> 5 -> 9 */
                return;
            } else {                              /* overflow carry */
                gen.idx[pos] = 0;
                pos--;
            }
        }
    }
}

/* initialize the generator on first use */
static void gen_init(void)
{
    gen.len = 1;
    for (size_t i = 0; i < 64; ++i)
        gen.idx[i] = 0;
    gen.idx[0] = 0;         /* first digit starts at 2 */
}

/* ensure that the cache holds at least n terms */
static void ensure_cache(idx_t n)
{
    if (cached >= (size_t)n)
        return;

    if (cap == 0) {
        cap = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    if (cached == 0)   /* first call: initialize generator */
        gen_init();

    while (cached < (size_t)n) {
        /* build number from current indices */
        seq_t num = first_digits[gen.idx[0]];
        for (size_t i = 1; i < gen.len; ++i)
            num = num * (seq_t)10 + all_digits[gen.idx[i]];

        if (is_prime_big(num)) {
            if (cached == cap) {
                cap *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[cached++] = num;
        }
        gen_next();
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n<1, placeholder */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
