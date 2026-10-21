/*
 * A256087  Non-palindromic balanced numbers in base 7.
 * https://oeis.org/A256087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1773, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_palindrome_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Balanced (non‑palindromic) base‑7 numbers.
   Term a(n) is the nth positive integer (n ≥ 1) whose base‑7
   representation satisfies Σ dᵢ·(k‑1‑2i) = 0 and is not a palindrome.
   The sequence is produced in increasing order by brute‑force
   enumeration of numbers of increasing base‑7 length. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_palindrome_digits.h"

/* cache for the sequence values */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* largest base‑7 length that has already been enumerated */
static int max_len_done = 0;

/* current digit vector (most significant first) */
static int digits[64];

/* append a value to the cache, growing it if necessary */
static void push_cache(seq_t val)
{
    if (cache_len == cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* recursive enumeration of all balanced, non‑palindromic numbers
   of a given base‑7 length.
   Parameters:
       len   – total number of digits
       pos   – current position (0 … len‑1)
       val   – numeric value of the already chosen prefix
       sum   – weighted sum of the already chosen prefix         */
static void enumerate_rec(int len, int pos, seq_t val, int sum)
{
    if (pos == len)
    {
        if (sum == 0 && !is_palindrome_digits(digits, (size_t)len))
            push_cache(val);
        return;
    }

    int start = (pos == 0) ? 1 : 0;
    for (int d = start; d <= 6; ++d)
    {
        digits[pos] = d;
        int weight = len - 1 - 2 * pos;
        int new_sum = sum + d * weight;
        seq_t new_val = val * 7 + d;
        enumerate_rec(len, pos + 1, new_val, new_sum);
    }
}

/* enumerate all valid numbers of the given length */
static void enumerate_len(int len)
{
    enumerate_rec(len, 0, 0, 0);
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n < 1 – return 0 */
        return (seq_t)0;

    while (cache_len < (size_t)n)
    {
        ++max_len_done;
        enumerate_len(max_len_done);
    }

    return cache[n - 1];
}
