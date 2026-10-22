/*
 * A357142  Nonnegative numbers all of whose pairs of consecutive decimal
 *          digits are adjacent digits, where 9 and 0 are considered adjacent.
 * https://oeis.org/A357142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   required_length
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose every pair of consecutive decimal digits are adjacent
   digits (difference ±1 modulo 10, treating 9 and 0 as adjacent).
   The sequence is obtained by enumerating all such numbers of
   increasing length and appending them in ascending order.
   a(1) = 0, a(2) = 1, … */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/required_length.h"

static seq_t *seq_cache = NULL;   /* dynamically grown array of values   */
static size_t seq_len   = 0;       /* number of values currently stored  */
static size_t seq_cap   = 0;       /* capacity of the array              */
static int    max_len_generated = 0;  /* maximum digit length generated    */

/* ensure the array has enough capacity for 'needed' entries */
static void ensure_cap_seq(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();               /* out‑of‑memory, stop */
    seq_cache = tmp;
    seq_cap   = new_cap;
}

/* append one value to the sequence array */
static void push_seq(seq_t v)
{
    ensure_cap_seq(seq_len + 1);
    seq_cache[seq_len++] = v;
}

/* recursive construction of all numbers of a given length that start
   with a specified digit.  'pos' counts how many digits have been
   fixed (starting at 1). */
static void build_len(int len, int last_digit, int pos, seq_t val)
{
    if (pos == len) {
        push_seq(val);
        return;
    }
    int d1 = (last_digit + 9) % 10;   /* left neighbour (digit-1)   */
    int d2 = (last_digit + 1) % 10;   /* right neighbour (digit+1)  */
    int first  = d1 < d2 ? d1 : d2;
    int second = d1 < d2 ? d2 : d1;
    build_len(len, first,  pos + 1, val * 10 + first);
    build_len(len, second, pos + 1, val * 10 + second);
}

/* generate all numbers of a specific length */
static void generate_len(int len)
{
    if (len == 1) {
        for (int d = 0; d <= 9; ++d)
            push_seq((seq_t)d);
    } else {
        for (int d = 1; d <= 9; ++d)   /* leading digit cannot be 0 */
            build_len(len, d, 1, (seq_t)d);
    }
}

/* ensure that seq_cache contains all numbers up to the length
   necessary for index n */
static void ensure_cache(idx_t n)
{
    if (seq_len >= (size_t)n) return;           /* already have enough */

    int needed = required_length(n);
    if (needed <= max_len_generated) return;    /* nothing new to generate */

    for (int len = max_len_generated + 1; len <= needed; ++len)
        generate_len(len);

    max_len_generated = needed;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */
    ensure_cache(n);
    return seq_cache[n - 1];
}
