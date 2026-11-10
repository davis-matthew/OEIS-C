/*
 * A280968  a(n) = A001097(n) + A001097(n+1).
 * https://oeis.org/A280968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001097(n) + A001097(n+1); computed by scanning for
 *            twin-prime members
 * Range:     u8 n <= 19, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A280968_is_prime(uint64_t n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

static bool A280968_is_twin(uint64_t p) {
    if (p < 3) return false;
    if (!A280968_is_prime(p)) return false;
    return A280968_is_prime(p + 2) || A280968_is_prime(p - 2);
}

seq_t a(idx_t n) {
    uint64_t count = 0;
    uint64_t c = 1;
    uint64_t first = 0, second = 0;
    uint64_t target = (uint64_t)n;
    while (count < target + 1) {
        c += 2;
        if (A280968_is_twin(c)) {
            count++;
            if (count == target) first = c;
            else if (count == target + 1) second = c;
        }
    }
    return (seq_t)first + (seq_t)second;
}
