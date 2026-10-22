/*
 * A329027  The least missing digit in the primorial base expansion of n. Only
 *          significant digits are considered, as the leading zeros are
 *          ignored.
 * https://oeis.org/A329027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least missing digit in the primorial base expansion of n
 * Range:     u8 n <= 731, u16 n <= 9357, u32 n <= 9357, u64 n <= 9357,
 *            u128 n <= 9357, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* n is the index; the value we need is the primorial-base expansion of n itself */
    if (n == 0) return (seq_t)0;          /* not used but safe */

    seq_t number = (seq_t)n;

    /* arrays to hold primorials and digits; 64 entries are more than enough for n up to 10^6 */
    seq_t primorials[64];
    size_t primorial_cnt = 0;
    primorials[primorial_cnt++] = (seq_t)1;  /* P0 = 1 */

    /* first prime is 2, generate primes on the fly */
    uint64_t p = 2;
    seq_t product = (seq_t)1;

    while (1) {
        seq_t new_product = product * (seq_t)p;
        if (new_product > number) break;    /* stop when next primorial would exceed n */

        primorials[primorial_cnt++] = new_product;
        product = new_product;

        /* find next prime after p */
        uint64_t next = p + 1;
        while (1) {
            bool isprime = true;
            if (next <= 2) {
                if (next == 2) { break; }
                next = 3;
            }
            if (next % 2 == 0) { next++; continue; }
            for (uint64_t q = 3; q * q <= next; q += 2) {
                if (next % q == 0) { isprime = false; break; }
            }
            if (isprime) break;
            next += 2;
        }
        p = next;
    }

    /* compute digits of the primorial expansion */
    size_t dcount = primorial_cnt;
    seq_t digits[64];
    seq_t rem = number;
    for (int i = (int)dcount - 1; i >= 0; --i) {
        seq_t base = primorials[i];
        seq_t d = rem / base;
        rem = rem % base;
        digits[i] = d;
    }

    /* find the least missing digit */
    for (unsigned long long cand = 0;; ++cand) {
        seq_t cand_seq = (seq_t)cand;
        bool found = false;
        for (size_t j = 0; j < dcount; ++j) {
            if (digits[j] == cand_seq) {
                found = true;
                break;
            }
        }
        if (!found) return cand_seq;   /* minimal missing digit found */
    }
}
