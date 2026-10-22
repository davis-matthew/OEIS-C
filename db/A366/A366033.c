/*
 * A366033  Successive digits of consecutive terms of the prime-counting
 *          function A000720.
 * https://oeis.org/A366033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, base
 * Formula:   not stated
 * Range:     u8 n <= 989, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, is_prime_small, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth digit of the consecutive concatenation of the prime‑counting
 * function values π(1), π(2), π(3), … .  The function returns the digit
 * (0–9) at position n (0‑based).  It is memoised so that calls in any
 * order are efficient. */
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* static state is preserved across calls */
    static idx_t   cur_k      = 0;   /* current integer x whose π(x) is known */
    static seq_t   pi_count   = 0;   /* current value of π(x) */
    static idx_t   cum_digits = 0;   /* total digits already processed */
    static seq_t   last_val   = 0;   /* π(cur_k) last processed */
    static int     last_len   = 0;   /* number of digits in last_val */

    /* Extend the processed range until the desired digit lies within it */
    while (cum_digits <= n) {
        ++cur_k;
        if (is_prime_small((seq_t)cur_k))
            ++pi_count;

        last_val = pi_count;
        last_len = digit_len(last_val);
        cum_digits += last_len;
    }

    /* offset of the required digit inside last_val (0‑based) */
    idx_t offset = n - (cum_digits - last_len);
    /* exponent of the power of ten that isolates the desired digit */
    idx_t exp = (idx_t)(last_len - 1 - offset);

    seq_t divisor = pow10(exp);
    seq_t digit   = (last_val / divisor) % (seq_t)10;
    return digit;
}
