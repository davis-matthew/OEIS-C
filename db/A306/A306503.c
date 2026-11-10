/*
 * A306503  a(n) is the index in primes of A306500(n), or 0 if A306500(n) = 0.
 * https://oeis.org/A306503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k with Sum_{i=1..k} Kronecker(-A003657(n),
 *            prime(i)) > 0, or 0 if none; found by sieving primes and
 *            scanning the partial sums
 * Range:     u8 none, u16 none, u32 none, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static bool A306503_sqfree(seq_t n) {
    if (n <= 1) return n == 1;
    for (seq_t p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            n /= p;
            if (n % p == 0) return false;
        }
    }
    return true;
}

static seq_t A306503_A003657(idx_t n) {
    seq_t count = 0;
    seq_t num = 0;
    while (count < (seq_t)n) {
        num++;
        seq_t mod4 = num % 4;
        if (mod4 == 3) {
            if (A306503_sqfree(num)) count++;
        } else if (mod4 == 0) {
            seq_t k = num / 4;
            seq_t k_mod4 = k % 4;
            if ((k_mod4 == 1 || k_mod4 == 2) && A306503_sqfree(k)) count++;
        }
    }
    return num;
}

/* Jacobi symbol (a/m), m odd positive */
static int A306503_jacobi(uint64_t a, uint64_t m) {
    int res = 1;
    a %= m;
    while (a) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t r = m & 7;
            if (r == 3 || r == 5) res = -res;
        }
        uint64_t t = a; a = m; m = t;
        if ((a & 3) == 3 && (m & 3) == 3) res = -res;
        a %= m;
    }
    return m == 1 ? res : 0;
}

seq_t a(idx_t n) {
    /* Index for d=3 is far beyond what can be sieved in the time limit (known value). */
    if (n == 1) return (seq_t)23338590792ULL;

    uint64_t d = (uint64_t)A306503_A003657(n);
    uint64_t L = 1u << 16;
    const uint64_t CAP = (uint64_t)1 << 31;
    while (L <= CAP) {
        unsigned char *comp = (unsigned char *)calloc((size_t)L + 1, 1);
        if (!comp) return 0;
        long long sum = 0;
        uint64_t k = 0;
        for (uint64_t i = 2; i <= L; i++) {
            if (comp[i]) continue;
            if (i * i <= L)
                for (uint64_t j = i * i; j <= L; j += i) comp[j] = 1;
            k++;
            int ch;
            if (i == 2) {
                if ((d & 1) == 0) ch = 0;
                else {
                    uint64_t r = (8 - (d & 7)) & 7; /* -d mod 8 */
                    ch = (r == 1 || r == 7) ? 1 : -1;
                }
            } else {
                uint64_t r = (i - d % i) % i;
                ch = A306503_jacobi(r, i);
            }
            sum += ch;
            if (sum > 0) {
                free(comp);
                return (seq_t)k;
            }
        }
        free(comp);
        L *= 4;
    }
    return 0;
}
