/*
 * A252746  Number of zeros on each row of irregular tables A252743 and
 *          A252744.
 * https://oeis.org/A252746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = number of m in [2^(n-1), 2^n - 1] (m = 0 for n = 0) such
 *            that, with v = A005940(m+1), the left child A005940(2m+1) =
 *            A003961(v) is less than the right child A005940(2m+2) = 2*v;
 *            counted by direct search over the tree level
 * Range:     u8 n <= 15, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 23 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A252746_primes[80];
static bool A252746_ready = false;

static void A252746_init(void) {
    if (A252746_ready) return;
    int cnt = 0;
    uint64_t c = 2;
    while (cnt < 80) {
        bool p = true;
        for (int j = 0; j < cnt; j++) {
            if (A252746_primes[j] * A252746_primes[j] > c) break;
            if (c % A252746_primes[j] == 0) { p = false; break; }
        }
        if (p) A252746_primes[cnt++] = c;
        c++;
    }
    A252746_ready = true;
}

static unsigned __int128 A252746_gcd(unsigned __int128 x, unsigned __int128 y) {
    while (y != 0) {
        unsigned __int128 t = x % y;
        x = y;
        y = t;
    }
    return x;
}

/* For v = A005940(m+1): is A003961(v) < 2*v ? */
static bool A252746_left_less(uint64_t m) {
    unsigned __int128 num = 1, den = 1;
    int i = 1;
    while (m > 0) {
        if (m & 1) {
            num *= A252746_primes[i];     /* prime(i+1) */
            den *= A252746_primes[i - 1]; /* prime(i)   */
            if (num >= 2 * den) return false; /* ratio only grows */
            unsigned __int128 g = A252746_gcd(num, den);
            if (g > 1) { num /= g; den /= g; }
            m >>= 1;
        } else {
            m >>= 1;
            i++;
        }
    }
    return num < 2 * den;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    A252746_init();
    uint64_t lo = (uint64_t)1 << (n - 1);
    uint64_t hi = ((uint64_t)1 << n) - 1;
    uint64_t count = 0;
    for (uint64_t m = lo; ; m++) {
        if (A252746_left_less(m)) count++;
        if (m == hi) break;
    }
    return (seq_t)count;
}
