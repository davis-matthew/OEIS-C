/*
 * A345113  a(n) is the palindrome reached after A345112(n) steps under
 *          repeated applications of the map x -> A345111(x), starting with n,
 *          or 0 if no palindrome is ever reached.
 * https://oeis.org/A345113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 38, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   digit_len, is_palindrome, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A345113: start with n, repeatedly apply
   x = x + left_rotate_decimal(x) until the result is a
   decimal palindrome.  Returns 0 if no palindrome is
   reached after 5000 iterations.  The left rotation
   moves the most significant digit to the least
   significant position. */
#include "../utils/digit_len.h"
#include "../utils/pow_int.h"
#include "../utils/is_palindrome.h"

/* Compute left cyclic shift of decimal digits of x */
static seq_t left_shift(seq_t x)
{
    if (x < 10U)                  /* one–digit numbers stay the same */
        return x;

    int  len   = digit_len(x);             /* number of decimal digits */
    seq_t pow10 = pow_int(10, (idx_t)(len - 1)); /* 10^(len-1) */

    seq_t msd = x / pow10;                 /* most significant digit */
    seq_t rest = x - msd * pow10;          /* remaining part */

    return rest * 10 + msd;                /* left rotation */
}

seq_t a(idx_t n)
{
    if (n == 0U)
        return 0U;                         /* safety for offset 0 */

    seq_t cur = (seq_t)n;
    const idx_t MAX_STEPS = 5000U;         /* empirical bound for n≤10000 */

    for (idx_t steps = 0; steps < MAX_STEPS; ++steps)
    {
        cur += left_shift(cur);            /* apply map x → x + left_shift(x) */
        if (is_palindrome(cur, 10U))
            return cur;                    /* palindrome found */
    }
    return 0U;                             /* no palindrome within limit */
}
