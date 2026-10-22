/*
 * A394690  Smallest k such that k * n is a Belgian-0 number.
 * https://oeis.org/A394690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   A394690(n) = smallest k ≥ 1 such that k·n is a Belgian-0 number.
 *            A number M is Belgian-0 iff for some prefix sum p of its decimal
 *            digits (including p=0) we have (M−p) ≡ 0 (mod digit_sum(M)). The
 *            implementation tests k=1,2,… until k·n satisfies that property
 * Range:     u8 n <= 42, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_digits.h"

/* Test whether x is a Belgian-0 number. */
static bool is_belgian0(seq_t x)
{
    /* Extract digits (least significant first). */
    uint8_t buf[64];
    size_t len;
    get_digits(x, buf, &len);

    /* No digits? x==0 ⇒ trivial but not used. */
    if (len == 0)
        return false;

    /* Build MS‑first digit array and compute digit sum. */
    uint8_t digits[64];
    uint64_t digit_sum = 0;
    for (size_t i = 0; i < len; ++i) {
        digits[i] = buf[len - 1 - i];
        digit_sum += digits[i];
    }

    /* If digit sum is 0, no Belgian‑0 number exists (should not happen). */
    if (digit_sum == 0)
        return false;

    /* Compute x mod digit_sum using the digit representation. */
    uint64_t rem = 0;
    for (size_t i = 0; i < len; ++i) {
        rem = (rem * 10 + digits[i]) % digit_sum;
    }

    /* Check prefix r=0 (full cycles only). */
    if (rem == 0)
        return true;

    /* Check prefixes r=1..len. */
    uint64_t pref = 0;  /* prefix sum of digits processed so far */
    for (size_t r = 1; r <= len; ++r) {
        pref += digits[r - 1];
        uint64_t diff = (rem + digit_sum - (pref % digit_sum)) % digit_sum;
        if (diff == 0)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Work with k as unsigned integer. */
    for (idx_t k = 1;; ++k) {
        seq_t m = (seq_t)k * (seq_t)n;
        if (is_belgian0(m))
            return (seq_t)k;
    }
}
