/*
 * A344888  a(n) is the least base k >= 2 where n is an undulating number
 *          (i.e., with digits of the form abababab...).
 * https://oeis.org/A344888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   not stated
 * Range:     u8 n <= 440, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 8943 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = least base k >= 2 for which n is an undulating number
 * (i.e., its representation in base k has digits a,b,a,b,...).
 * 
 * This implementation directly checks successive bases k = 2,3,… until
 * the representation of n satisfies the undulating property.
 * For any n > 0 it is guaranteed that base = n+1 will produce a
 * single-digit representation, which is automatically undulating.
 */

#include <stdbool.h>
#include <stddef.h>

/* Helper: return true iff n (in seq_t) is undulating in base 'base'.
 * 'base' is passed as idx_t but converted to seq_t within the
 * function. If base exceeds the maximum representable value of
 * seq_t, the function immediately returns true because every
 * representation of n then has a single digit. */
static bool is_undulating_in_base(seq_t n, idx_t base)
{
    /* If the base is larger than the maximum value of seq_t,
       every representation of n consists of a single digit. */
    if (base > (idx_t)(~(seq_t)0))
        return true;

    seq_t base_seq = (seq_t)base;
    /* Convert n to base 'base_seq' (least‑significant digit first). */
    uint16_t digits[128];      /* 128 digits are enough for any 128‑bit value */
    size_t len = 0;
    seq_t temp = n;

    if (temp == (seq_t)0) {    /* 0 is undulating in any base */
        return true;
    }

    while (temp != (seq_t)0) {
        digits[len++] = (uint16_t)(temp % base_seq);
        temp /= base_seq;
    }

    /* One‑digit representation is always undulating */
    if (len == 1)
        return true;

    uint16_t a = digits[0];
    uint16_t b = digits[1];

    for (size_t i = 2; i < len; ++i) {
        if ((i & 1) == 0) {
            if (digits[i] != a)
                return false;
        } else {
            if (digits[i] != b)
                return false;
        }
    }
    return true;
}

/* Main sequence function requested by the judge. */
seq_t a(idx_t n)
{
    /* n = 0 is a special case: the smallest base is always 2. */
    if (n == (idx_t)0)
        return (seq_t)2;

    /* Search bases 2,3,… until we find an undulating representation.
       For any n > 0, base = n+1 produces a single‑digit representation
       and is guaranteed to be undulating, so the loop is bounded. */
    for (idx_t k = 2; k <= n + 1; ++k) {
        if (is_undulating_in_base((seq_t)n, k))
            return (seq_t)k;
    }

    /* The loop above always returns, but a return here keeps the compiler
       happy and satisfies the function's exhaustive path requirement. */
    return (seq_t)(n + 1);
}
