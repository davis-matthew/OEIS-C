/*
 * A249854  Numbers <= 10^6 with valid Luhn mod 10 check digit, sorted
 *          lexicographically.
 * https://oeis.org/A249854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 9252, u64 n <= 9252,
 *            u128 n <= 9252, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute by enumerating all integers <= 1e6 in lexicographic order
   and keeping those that satisfy the Luhn mod‑10 check. */
#include <stdbool.h>
#include <stdlib.h>

/* static cache for the pre‑computed sequence */
static seq_t     *seq       = NULL;    /* array of valid numbers   */
static size_t    seq_len   = 0;        /* number of elements stored*/
static size_t    seq_cap   = 0;        /* allocated capacity       */
static bool      seq_inited = false;  /* flag whether built        */
static const unsigned int LIMIT = 1000000u;

/* Luhn validation for an unsigned integer.
   Returns true iff the sum of the digits (with alternating doubling)
   is divisible by 10. */
static bool luhn_valid_uint(unsigned int x)
{
    unsigned int sum = 0;
    bool double_digit = false;
    while (x != 0) {
        unsigned int d = x % 10;
        if (double_digit) {
            d *= 2;
            if (d > 9) d -= 9;
        }
        sum += d;
        double_digit = !double_digit;
        x /= 10;
    }
    /* 0 is handled by the loop as sum==0 */
    return (sum % 10) == 0;
}

/* Append a value to the dynamic array, growing as necessary. */
static void push_seq(seq_t v)
{
    if (seq_len + 1 > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 1024;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq = tmp;
        seq_cap = new_cap;
    }
    seq[seq_len++] = v;
}

/* Depth‑first traversal that enumerates numbers in lexicographic order.
   val is guaranteed to be > 0 when entered. */
static void rec(unsigned int val)
{
    if (luhn_valid_uint(val))
        push_seq((seq_t)val);

    if (val > LIMIT / 10)
        return;

    for (unsigned int d = 0; d <= 9; ++d) {
        unsigned int newval = val * 10 + d;
        if (newval <= LIMIT)
            rec(newval);
    }
}

/* Build the complete sequence once and store it in the static array. */
static void build_seq(void)
{
    seq_inited = true;
    seq_cap = 0;
    seq_len = 0;
    seq = NULL;

    /* Handle the special value 0 first (lexicographically smallest). */
    if (luhn_valid_uint(0))
        push_seq((seq_t)0);

    /* Generate all numbers starting with each non‑zero first digit. */
    for (unsigned int first = 1; first <= 9; ++first)
        rec(first);
}

/* External function as required for the OEIS evaluator. */
seq_t a(idx_t n)
{
    if (!seq_inited)
        build_seq();

    if (n == 0 || (size_t)n > seq_len)
        return (seq_t)0;          /* out of range – not expected */

    return seq[(size_t)(n - 1)];
}
