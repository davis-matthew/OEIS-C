/*
 * A286302  Numbers n such that A133364(n) <= 1.
 * https://oeis.org/A286302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number k such that #{(m,p): k = m+p, m in A001694, p
 *            prime} <= 1; computed by sieving all sums m+p up to 20000
 *            (conjectured last term 10404)
 * Range:     u8 n <= 27, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define A286302_LIM 20000

static int A286302_terms[A286302_LIM + 1];
static int A286302_count = 0;
static bool A286302_ready = false;

static bool A286302_powerful(int m) {
    int p;
    for (p = 2; (long long)p * p <= m; p++) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            if (e < 2) return false;
        }
    }
    return m == 1 || false;
}

static void A286302_init(void) {
    static unsigned char comp[A286302_LIM + 1];
    static unsigned char cnt[A286302_LIM + 1];
    static int primes[A286302_LIM];
    int np = 0, i, j, m;
    for (i = 2; i <= A286302_LIM; i++) {
        if (!comp[i]) {
            primes[np++] = i;
            for (j = 2 * i; j <= A286302_LIM; j += i) comp[j] = 1;
        }
    }
    for (m = 1; m < A286302_LIM; m++) {
        bool pw;
        if (m == 1) pw = true;
        else {
            /* powerful check: every prime factor has exponent >= 2 */
            int t = m, p;
            pw = true;
            for (p = 2; (long long)p * p <= t; p++) {
                if (t % p == 0) {
                    int e = 0;
                    while (t % p == 0) { t /= p; e++; }
                    if (e < 2) { pw = false; break; }
                }
            }
            if (pw && t != 1) pw = false;
        }
        if (!pw) continue;
        for (i = 0; i < np && m + primes[i] <= A286302_LIM; i++) {
            int s = m + primes[i];
            if (cnt[s] < 2) cnt[s]++;
        }
    }
    for (i = 1; i <= A286302_LIM; i++)
        if (cnt[i] <= 1) A286302_terms[A286302_count++] = i;
    A286302_ready = true;
}

seq_t a(idx_t n) {
    if (!A286302_ready) A286302_init();
    if (n < 1 || n > (idx_t)A286302_count) return (seq_t)0;
    return (seq_t)A286302_terms[n - 1];
}
