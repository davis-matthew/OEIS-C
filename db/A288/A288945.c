/*
 * A288945  Starting with a(1) = 1, a(n) = smallest nonnegative integer not
 *          yet in the sequence such that the first digit of a(n) is the last
 *          digit of a(n-1) plus or minus 1, with preference given to -1 when
 *          possible. The digit 0 is not allowed.
 * https://oeis.org/A288945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1,
   a(n) = smallest unused non‑negative integer not yet in the sequence
   whose first digit equals the last digit of a(n-1) minus 1 (if ≥1) or
   plus 1 otherwise; zero digit is forbidden. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static state for memoisation */
static seq_t *seq = NULL;         /* sequence storage                */
static size_t seq_len = 0;        /* number of terms currently stored */
static size_t seq_cap = 0;        /* current capacity of 'seq'       */
static seq_t next_start[10];     /* next unchecked candidate per first digit */

/* initialise the sequence on first use */
static void init_sequence(void)
{
    seq_cap = 32;
    seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    seq[0] = (seq_t)1;
    seq_len = 1;
    for (int d = 0; d < 10; ++d)
        next_start[d] = (seq_t)1;
}

/* return true if x contains the digit 0 (x>0) */
static bool has_zero(seq_t x)
{
    if (x == (seq_t)0) return true;
    while (x > (seq_t)0) {
        if ((x % (seq_t)10) == (seq_t)0)
            return true;
        x /= (seq_t)10;
    }
    return false;
}

/* return first decimal digit of x (x>0) */
static int first_digit(seq_t x)
{
    while (x >= (seq_t)10)
        x /= (seq_t)10;
    return (int)x;
}

/* linear search whether v is already in the sequence */
static bool is_used(seq_t v)
{
    for (size_t i = 0; i < seq_len; ++i)
        if (seq[i] == v)
            return true;
    return false;
}

/* find the smallest unused number with first digit f (1..9),
   no zero digits, starting the search from next_start[f]. */
static seq_t get_first_candidate(int f)
{
    seq_t cand = next_start[f];
    for (;;) {
        if (!has_zero(cand) && first_digit(cand) == f && !is_used(cand)) {
            next_start[f] = cand + (seq_t)1;
            return cand;
        }
        ++cand;
    }
}

/* main function required by the framework */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;          /* undefined behaviour for n<1 */

    if (seq_len == 0) init_sequence();

    /* convert n to size_t safely for the loop; n is small enough for typical tests */
    size_t target = (size_t)n;

    while (seq_len < target) {
        seq_t prev = seq[seq_len - 1];
        int last_digit = (int)(prev % (seq_t)10);
        int next_fd = (last_digit > 1) ? last_digit - 1 : last_digit + 1;

        seq_t cand = get_first_candidate(next_fd);

        /* grow array if needed */
        if (seq_len == seq_cap) {
            seq_cap = seq_cap * 2;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len++] = cand;
    }

    return seq[target - 1];
}
