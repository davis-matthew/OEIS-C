/*
 * A369800  Lexicographically earliest sequence of distinct positive integers
 *          such that the parity of the successive bits of the concatenated
 *          binary expansions of the terms is equal to the parity of the
 *          terms.
 * https://oeis.org/A369800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A369800: lexicographically earliest permutation of positive integers
 * such that the parity of the nth term equals the parity of the nth bit
 * of the concatenated binary expansions of the terms. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* forward declaration of helper functions */
static unsigned bitlen_seq_t(seq_t x);
static void append_bits(seq_t x);
static void grow_cache(idx_t needed);
static void grow_used(idx_t needed);
static void grow_bits(idx_t needed);

/* Global mutable state for memoisation */
static seq_t *seq_cache = NULL;       /* cache of computed sequence values */
static size_t cache_size = 0;         /* allocated capacity of seq_cache   */
static idx_t cache_len = 0;           /* number of terms already computed */

static char *used = NULL;             /* boolean array of used numbers */
static size_t used_size = 0;          /* allocated capacity of used array */

static char *bits = NULL;             /* concatenated binary bits 1/0 */
static size_t bits_size = 0;          /* allocated capacity of bits */
static size_t bits_len = 0;           /* number of bits stored      */

static seq_t next_odd = 1;            /* next unused odd candidate   */
static seq_t next_even = 2;           /* next unused even candidate   */

/* return bit length of x (for unsigned integers) */
static unsigned bitlen_seq_t(seq_t x)
{
    unsigned len = 0;
    while (x > 0) {
        ++len;
        x >>= 1;
    }
    return len ? len : 1;        /* for x == 0, return 1 (not used) */
}

/* append the bits of x (MSB first) to the global bits array */
static void append_bits(seq_t x)
{
    unsigned len = bitlen_seq_t(x);
    grow_bits(bits_len + len);
    for (int i = (int)len - 1; i >= 0; --i) {
        bits[bits_len++] = (char)((x >> i) & 1);
    }
}

/* enlarge seq_cache to hold at least 'needed' elements */
static void grow_cache(idx_t needed)
{
    if ((size_t)needed <= cache_size) return;
    size_t new_size = cache_size ? cache_size * 2 : 128;
    while (new_size < (size_t)needed) new_size *= 2;
    seq_t *tmp = (seq_t*)realloc(seq_cache, new_size * sizeof(seq_t));
    if (!tmp) abort(); /* out of memory */
    seq_cache = tmp;
    cache_size = new_size;
}

/* enlarge used array to index up to 'needed' */
static void grow_used(idx_t needed)
{
    if ((size_t)needed < used_size) return;
    size_t new_size = used_size ? used_size * 2 : 128;
    while (new_size <= (size_t)needed) new_size *= 2;
    char *tmp = (char*)realloc(used, new_size * sizeof(char));
    if (!tmp) abort(); /* out of memory */
    /* initialise new cells to 0 */
    memset(tmp + used_size, 0, new_size - used_size);
    used = tmp;
    used_size = new_size;
}

/* enlarge bits array to hold at least 'needed' bits */
static void grow_bits(idx_t needed)
{
    if ((size_t)needed <= bits_size) return;
    size_t new_size = bits_size ? bits_size * 2 : 128;
    while (new_size < (size_t)needed) new_size *= 2;
    char *tmp = (char*)realloc(bits, new_size * sizeof(char));
    if (!tmp) abort(); /* out of memory */
    bits = tmp;
    bits_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not used, offset is 1 */

    /* generate terms up to n */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        /* determine required parity for a(k) */
        int parity_bit;
        if (k == 1) {
            /* first bit of the stream is 1 (MSB of 1) */
            parity_bit = 1;
        } else if (k == 2) {
            /* k > L_{k-1}, so first bit of new term, which is 1 */
            parity_bit = 1;
        } else {
            /* L_{k-1} >= k for k>=3, so bit is already known */
            if (bits_len < (size_t)k) {
                /* safety fallback – should not happen */
                parity_bit = 1;
            } else {
                parity_bit = bits[k - 1];
            }
        }

        /* pick smallest unused number with this parity */
        seq_t val;
        if (parity_bit) {                /* odd */
            while (1) {
                grow_used((idx_t)next_odd);
                if (!used[(idx_t)next_odd]) { val = next_odd; break; }
                next_odd += 2;
            }
        } else {                         /* even */
            while (1) {
                grow_used((idx_t)next_even);
                if (!used[(idx_t)next_even]) { val = next_even; break; }
                next_even += 2;
            }
        }

        /* mark as used */
        grow_used((idx_t)val);
        used[(idx_t)val] = 1;

        /* update pointers for next search */
        if (parity_bit) {
            if (next_odd == val) next_odd += 2;
        } else {
            if (next_even == val) next_even += 2;
        }

        /* append binary representation of val to bits stream */
        append_bits(val);

        /* store in cache */
        grow_cache(k);
        seq_cache[k - 1] = val;
    }

    cache_len = n;
    return seq_cache[n - 1];
}
