/*
 * A256866  Numbers divisible by prime(d+1) for each digit d of their base-6
 *          representation.
 * https://oeis.org/A256866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the n‑th non‑negative integer m such that
   for every base‑6 digit d appearing in m, m is divisible by prime(d+1). */

#include <stdbool.h>

/* Prime numbers for digits 0…5: prime(d+1) */
static const seq_t _primes[6] = {
    (seq_t)2,  /* prime(1)   */
    (seq_t)3,  /* prime(2)   */
    (seq_t)5,  /* prime(3)   */
    (seq_t)7,  /* prime(4)   */
    (seq_t)11, /* prime(5)   */
    (seq_t)13  /* prime(6)   */
};

/* Return true if m satisfies the digit‑divisibility rule. */
static bool _valid(idx_t m) {
    unsigned char seen[6] = {0};
    idx_t tmp = m;
    /* Handle 0 – the while loop is skipped, resulting in seen unchanged,
       so 0 is considered valid (0 % p == 0 for any prime). */
    while (tmp > (idx_t)0) {
        unsigned int d = (unsigned int)(tmp % (idx_t)6);
        if (!seen[d]) {
            if (m % _primes[d] != (seq_t)0) {
                return false;
            }
            seen[d] = 1;
        }
        tmp /= (idx_t)6;
    }
    return true;
}

seq_t a(idx_t n) {
    idx_t count = (idx_t)0;
    idx_t m = (idx_t)0;
    while (true) {
        if (_valid(m)) {
            count++;
            if (count == n) {
                return (seq_t)m;
            }
        }
        m++;
    }
}
