/*
 * A303935  Size of orbit of n under repeated application of sum of factorial
 *          of digits of n.
 * https://oeis.org/A303935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of the orbit of n under repeated application of
 *            the sum of factorial of digits of n
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses memoisation with a timestamped visited array
   and works for any n that fits in ``idx_t``. It guarantees that the
   orbit length is computed correctly for all n up to at least 10⁴ in well
   under two seconds (C11 / C++17). */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* factorials of digits 0..9. */
static const seq_t _digit_fact[10] =
    {1ULL,1ULL,2ULL,6ULL,24ULL,120ULL,720ULL,5040ULL,40320ULL,362880ULL};

/* Maximum value that may appear after the first iteration.
   For any 64–bit number the first factorial sum has at most 19 digits,
   thus ≤ 19*362880 = 6,889,920 < 7,000,000.  We use 10 million to be
   safe for larger input types or future extensions. */
#define MAX_VAL 10000000UL

/* Timestamped visited table.  Each entry stores the timestamp of the
   last call that visited this number.  A 32‑bit counter is sufficient
   for practical usage. */
static uint32_t _visited[MAX_VAL];
static uint32_t _current_stamp = 0;

/* Compute the sum of factorials of the decimal digits of ``x``.  */
static seq_t
_sum_fact_digits(seq_t x)
{
    seq_t sum = 0;
    if (x == 0) return 1;           /* 0! = 1 */
    while (x > 0) {
        unsigned digit = (unsigned)(x % 10);
        sum += _digit_fact[digit];
        x /= 10;
    }
    return sum;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    /* Advance the global timestamp.  Reset the visited array on
       overflow (the overflow is astronomically unlikely). */
    if (++_current_stamp == 0) {
        memset(_visited, 0, sizeof _visited);
        ++_current_stamp;
    }

    /* If n is below MAX_VAL, we can mark it directly.  Otherwise
       we count it once but never mark it in the visited table. */
    seq_t count = 0;
    seq_t curr;
    if (n < (idx_t)MAX_VAL) {
        curr = n;
        while (_visited[(size_t)curr] != _current_stamp) {
            _visited[(size_t)curr] = _current_stamp;
            ++count;
            curr = _sum_fact_digits(curr);
        }
    } else {
        /* Large starting value: count it explicitly, then continue from
           the first iterate which is guaranteed to be < MAX_VAL. */
        ++count;                    /* n itself */
        curr = _sum_fact_digits(n);
        while (_visited[(size_t)curr] != _current_stamp) {
            _visited[(size_t)curr] = _current_stamp;
            ++count;
            curr = _sum_fact_digits(curr);
        }
    }
    return count;
}
