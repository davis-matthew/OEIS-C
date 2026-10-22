/*
 * A347401  Lexicographically earliest sequence of distinct terms > 0 such
 *          that the sum n + a(n) forms a palindrome in base 10.
 * https://oeis.org/A347401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   bit_off, byte_idx, is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A347401 – Lexicographically earliest sequence of distinct terms > 0
 * such that n + a(n) is a base‑10 palindrome.
 *
 * The sequence is generated greedily: for each index n (starting at 1),
 * a(n) is the smallest positive integer that has not appeared earlier and
 * makes the sum n + a(n) a decimal palindrome.
 *
 * The function stores all values it has computed in a static array so that
 * repeated calls with arbitrary indices are fast.  A dynamic bitset keeps
 * track of already used values, ensuring the distinctness property.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_palindrome.h"   /* boolean function bool is_palindrome(seq_t, idx_t) */
#include "../utils/bit_off.h"
#include "../utils/byte_idx.h"

static seq_t *vals = NULL;
static size_t vals_cap = 0;          /* number of seq_t slots allocated   */
static idx_t  vals_max = 0;          /* largest index for which a value
                                      * has been computed                */

static uint8_t *used_bits = NULL;    /* bitset: used_bits[bit/8] holds the
                                      * status of value bit+1             */
static size_t used_bytes = 0;        /* number of bytes allocated for used_bits */

/* ------------------------------------------------------------------ */
/* Bitset helpers                                                   */
     /* bit%8        */

static void ensure_used_cap(idx_t bit)
{
    size_t byte = byte_idx(bit);
    if (byte >= used_bytes) {
        size_t new_bytes = used_bytes? used_bytes * 2 : 16;
        while (new_bytes <= byte) new_bytes <<= 1;
        uint8_t *new_tb = (uint8_t*)calloc(new_bytes, 1);
        if (!new_tb) abort();                 /* allocation failure */
        if (used_bits) {
            memcpy(new_tb, used_bits, used_bytes);
            free(used_bits);
        }
        used_bits   = new_tb;
        used_bytes  = new_bytes;
    }
}

static inline bool is_used(idx_t x)
{
    if (x == 0) return false;                 /* 0 is never used */
    size_t bit = (size_t)x;
    size_t byte = byte_idx(bit);
    if (byte >= used_bytes) return false;
    return (used_bits[byte] >> bit_off(bit)) & 1;
}

static inline void set_used(idx_t x)
{
    if (x == 0) return;
    size_t bit = (size_t)x;
    ensure_used_cap(bit);
    used_bits[byte_idx(bit)] |= 1U << bit_off(bit);
}

/* ------------------------------------------------------------------ */
/* Value array helpers                                              */
/* ------------------------------------------------------------------ */
static void ensure_vals_cap(idx_t n)
{
    size_t required = (size_t)n + 1;          /* +1 to be able to index n */
    if (required <= vals_cap) return;
    size_t new_cap = vals_cap? vals_cap * 2 : 64;
    while (new_cap < required) new_cap <<= 1;
    seq_t *new_vals = (seq_t*)realloc(vals, new_cap * sizeof(seq_t));
    if (!new_vals) abort();                   /* allocation failure */
    vals     = new_vals;
    vals_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Main function                                                    */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* underflow is undefined (offset 1) */

    /* Compute missing values up to n */
    if (n > vals_max) {
        ensure_vals_cap(n);
        for (idx_t i = vals_max + 1; i <= n; ++i) {
            seq_t cand = 1;                   /* smallest positive integer */
            while (1) {
                if (!is_used((idx_t)cand)) {
                    seq_t sum = (seq_t)i + cand;
                    if (is_palindrome(sum, 10)) break;   /* found a valid term */
                }
                ++cand;
            }
            vals[i] = cand;
            set_used((idx_t)cand);
        }
        vals_max = n;
    }
    return vals[n];
}
