/*
 * A321513  Numbers that are periodic in base 4, written in decimal, where the
 *          base-4 expansion contains at least two periods.
 * https://oeis.org/A321513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A321513
   a(n) = the n-th positive integer whose base‑4 expansion, when padded
   with an arbitrary number of leading zeros, consists of at least two
   identical consecutive periods (truncation of leading zeros is allowed). */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Return true if x has a base‑4 representation that can be written as
   P repeated at least twice, where P may contain leading zeros.  */
static bool is_periodic(seq_t x)
{
    if (x == 0) return false;          /* 0 is excluded */

    /* Collect base‑4 digits (least significant first).  */
    size_t cap = 16;
    size_t len = 0;
    uint8_t *digits = (uint8_t *)malloc(cap);
    if (digits == NULL) return false;   /* allocation failure */

    seq_t t = x;
    while (t != 0) {
        if (len == cap) {               /* grow buffer */
            size_t new_cap = cap * 2;
            uint8_t *tmp = (uint8_t *)realloc(digits, new_cap);
            if (tmp == NULL) {
                free(digits);
                return false;
            }
            digits = tmp;
            cap = new_cap;
        }
        digits[len++] = (uint8_t)(t % 4);
        t /= 4;
    }

    /* Reverse to most significant first.  */
    for (size_t i = 0; i < len / 2; ++i) {
        uint8_t tmp = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = tmp;
    }

    /* Try all period lengths.  */
    for (size_t L = 1; L <= len; ++L) {
        size_t k = (len + L - 1) / L;    /* minimal repetitions to cover len */
        if (k < 2) continue;             /* need at least two periods */
        size_t m = L * k;                /* total length after padding */
        size_t pad = m - len;            /* leading zeros added */

        bool ok = true;
        for (size_t i = 0; i + L < m && ok; ++i) {
            uint8_t a = (i < pad) ? 0 : digits[i - pad];
            size_t j = i + L;
            uint8_t b = (j < pad) ? 0 : digits[j - pad];
            if (a != b) ok = false;
        }

        if (ok) {
            free(digits);
            return true;
        }
    }

    free(digits);
    return false;
}

extern seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n < 1 */

    idx_t count = 0;
    seq_t candidate = 1;

    while (true) {
        if (is_periodic(candidate)) {
            ++count;
            if (count == n) return candidate;
        }
        ++candidate;
    }
}
