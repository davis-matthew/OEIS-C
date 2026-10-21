/*
 * A247616  Numbers with more than two distinct digits in arithmetic
 *          progression.
 * https://oeis.org/A247616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 40, u32 n <= 95, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumeration: a(n) is the nth smallest integer whose decimal digits
   form an arithmetic progression with a non‑zero common difference
   and contain at least three digits. */
#include <stdlib.h>
#include <stdbool.h>

/* Compare two seq_t values for qsort.  Works for any unsigned integer
   type declared as seq_t. */
static int seq_cmp(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Cache of all values.  Built only once. */
static seq_t *seq_cache = NULL;
static size_t seq_len   = 0;

/* Generate the full sequence up to 10 digits.  The maximum value is
   9876543210, which fits in all supported seq_t sizes. */
static void generate_sequence(void)
{
    if (seq_cache != NULL) return;   /* already built */

    size_t capacity = 64;            /* initial capacity */
    seq_cache = (seq_t *)malloc(capacity * sizeof(seq_t));
    seq_len   = 0;

    for (idx_t len = 3; len <= 10; ++len) {
        for (int a = 1; a <= 9; ++a) {        /* first digit */
            for (int d = -9; d <= 9; ++d) {
                if (d == 0) continue;        /* exclude constant‑digit numbers */

                int last = a + (int)(len - 1) * d;
                if (last < 0 || last > 9) continue;  /* out of digit range */

                seq_t num = 0;
                int digit = a;
                for (idx_t k = 0; k < len; ++k) {
                    num = num * 10 + digit;
                    digit += d;
                }

                if (seq_len == capacity) {
                    capacity <<= 1;
                    seq_cache = (seq_t *)realloc(seq_cache, capacity * sizeof(seq_t));
                }
                seq_cache[seq_len++] = num;
            }
        }
    }

    /* Resize to actual size and sort numerically. */
    seq_cache = (seq_t *)realloc(seq_cache, seq_len * sizeof(seq_t));
    qsort(seq_cache, seq_len, sizeof(seq_t), seq_cmp);
}

/* External function required by the tests. */
seq_t a(idx_t n)
{
    generate_sequence();

    if (n == 0 || n > seq_len) return 0;          /* out of range; behaviour irrelevant for 0 */
    return seq_cache[(size_t)(n - 1)];
}
