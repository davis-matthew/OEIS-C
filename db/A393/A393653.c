/*
 * A393653  ISBN-10 numbers with the same check digit upon conversion to
 *          ISBN-13.
 * https://oeis.org/A393653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   a(n) = 10*n + ch, where ch is the common ISBN‑10 * and ISBN‑13
 *            check digit of the 9‑digit number n
 * Range:     u8 n <= 3, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute the 10‑digit value for a 9‑digit number n.
 * If n does NOT satisfy the equal‑check‑digit property, ok = 0
 * and the return value is unspecified.  If it does, ok = 1
 * and the return value is 10*n + common check digit. */
static seq_t compute_value_for_n(seq_t n, int *ok)
{
    /* ISBN‑10 check digit */
    seq_t t = n;
    int sum10 = 0;
    for (int i = 0; i < 9; ++i) {
        int digit = (int)(t % 10);
        sum10 = (sum10 + digit * (9 - i)) % 11;
        t /= 10;
    }
    if (sum10 == 10) {          /* "X" is not allowed for the 9‑digit block */
        *ok = 0;
        return 0;
    }

    /* ISBN‑13 check digit for the number 978 + n (12 digits before the check) */
    int sum13 = 9 * 1 + 7 * 3 + 8 * 1;   /* contribution of the fixed prefix 978 */
    t = n;
    int weight = 3;                /* weight for the least significant digit */
    for (int i = 0; i < 9; ++i) {
        int digit = (int)(t % 10);
        sum13 = (sum13 + digit * weight) % 10;
        weight = (weight == 3) ? 1 : 3;  /* alternate 3,1 */
        t /= 10;
    }
    int ch13 = (10 - (sum13 % 10)) % 10;

    if (sum10 == ch13) {
        *ok = 1;
        return n * (seq_t)10 + (seq_t)sum10;
    } else {
        *ok = 0;
        return 0;
    }
}

static size_t cache_len = 0;
static seq_t cache[10001];          /* enough to store the first 10 000 terms */
static seq_t next_n = 0;            /* next 9‑digit number to test */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for offset‑free indices */

    /* Clamp to the maximum number we have prepared a static buffer for. */
    if (n > 10000)
        return 0;                    /* out of range for this implementation */

    while (cache_len < n) {
        int ok;
        seq_t val = compute_value_for_n(next_n, &ok);
        if (ok)
            cache[cache_len++] = val;
        ++next_n;
    }
    return cache[n - 1];
}
