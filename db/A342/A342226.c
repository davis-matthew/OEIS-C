/*
 * A342226  Numbers k with digits in nondecreasing order and satisfying k <
 *          (product of digits of k) * (sum of digits of k).
 * https://oeis.org/A342226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, fini
 * Formula:   a(n) = number k with digits in nondecreasing order and k <
 *            (product of digits of k) * (sum of digits of k)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* static cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of terms stored */
static size_t cache_cap  = 0;   /* allocated capacity */
static int current_length = 0;   /* next integer length to generate */

/* Ensure the cache has at least 'needed' slots */
static void grow_cache(size_t needed)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { /* allocation failed – terminate program */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Append a new term to the cache */
static void push_term(seq_t val)
{
    if (cache_size == cache_cap) grow_cache(cache_size + 1);
    cache[cache_size++] = val;
}

/* Recursive generation of nondecreasing-digit numbers of a fixed length.
   pos   : current position (0 .. len-1)
   last  : minimum digit allowed at this position
   value : number built so far
   sum   : sum of digits built so far
   prod  : product of digits built so far   */
static void gen_len_rec(int len, int pos, int last, seq_t value, seq_t sum, seq_t prod)
{
    if (pos == len) {
        /* test inequality: k < prod * sum  */
        if (sum != 0) {
            seq_t q = value / sum;      /* integer division */
            if (prod > q) push_term(value);
        }
        return;
    }

    for (int d = last; d <= 9; ++d) {
        seq_t new_val  = value * 10 + (seq_t)d;
        seq_t new_sum  = sum + d;
        seq_t new_prod = prod * (seq_t)d;
        gen_len_rec(len, pos + 1, d, new_val, new_sum, new_prod);
    }
}

/* Generate all numbers of the next integer length */
static void generate_next_length(void)
{
    ++current_length;
    gen_len_rec(current_length, 0, 1, 0, 0, 1);
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0, but return 0 safely */

    while (cache_size < n) generate_next_length();

    return cache[n - 1];
}
