/*
 * A305215  a(n) is the number of numbers whose largest prime power factor
 *          equals A000961(n).
 * https://oeis.org/A305215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(A051451(n)/A000961(n)) = Product_{prime p <= x, p
 *            != q} (floor(log_p x)+1), where x = A000961(n) is a prime power
 *            q^k (a(1)=1 for x=1); computed by sieve
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    uint64_t limit = 256;
    uint64_t x = 1, q = 0;
    for (;;) {
        uint64_t *spf = (uint64_t *)calloc((size_t)(limit + 1), sizeof(uint64_t));
        if (!spf) return 0;
        for (uint64_t i = 2; i <= limit; i++) {
            if (spf[i] == 0) {
                for (uint64_t j = i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
        uint64_t count = 0;
        bool found = false;
        for (uint64_t m = 1; m <= limit; m++) {
            bool pp = true;
            uint64_t pr = 0;
            if (m > 1) {
                uint64_t t = m;
                pr = spf[m];
                while (t > 1) {
                    if (spf[t] != pr) { pp = false; break; }
                    t /= pr;
                }
            }
            if (pp) {
                count++;
                if (count == (uint64_t)n) {
                    x = m;
                    q = pr;
                    found = true;
                    break;
                }
            }
        }
        if (found) {
            seq_t result = 1;
            for (uint64_t p = 2; p <= x; p++) {
                if (spf[p] != p || p == q) continue;
                unsigned e = 1;
                uint64_t pw = p;
                while (pw <= x / p) { pw *= p; e++; }
                result *= (seq_t)(e + 1);
            }
            free(spf);
            return result;
        }
        free(spf);
        limit *= 2;
    }
}
