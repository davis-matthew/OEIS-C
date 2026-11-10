/*
 * A262298  Primes in A262572.
 * https://oeis.org/A262298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search — a(n) = n-th prime among A262572(m) (concatenation of
 *            1..m+1 omitting 2), m >= 1; candidates are built incrementally,
 *            sieved by small primes, then tested by Miller-Rabin
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 1, u128 n <= 1,
 *            big any n
 * Validated: partial, pass, 2 terms through n = 2 (2026-11-07)
 * Helpers:   is_prime_u64, mulmod_seq, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod_seq.h"
#include "../utils/num_digits_idx.h"

#define A262298_NSP 300
static uint32_t A262298_sp[A262298_NSP];
static int A262298_nsp = 0;

static void A262298_init_small_primes(void) {
    if (A262298_nsp) return;
    for (uint32_t c = 3; A262298_nsp < A262298_NSP; c += 2) {
        bool pr = true;
        for (int i = 0; i < A262298_nsp; i++) {
            uint32_t p = A262298_sp[i];
            if (p * p > c) break;
            if (c % p == 0) { pr = false; break; }
        }
        if (pr) A262298_sp[A262298_nsp++] = c;
    }
}

static seq_t A262298_powmod(seq_t b, seq_t e, seq_t m) {
    seq_t r = 1;
    b %= m;
    while (e > 0) {
        if ((e & 1) != 0) r = mulmod_seq(r, b, m);
        e >>= 1;
        if (e > 0) b = mulmod_seq(b, b, m);
    }
    return r;
}

static bool A262298_mr(seq_t n) {
    static const uint32_t bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
    seq_t nm1 = n - 1;
    seq_t d = nm1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (int i = 0; i < (int)(sizeof(bases) / sizeof(bases[0])); i++) {
        seq_t x = A262298_powmod((seq_t)bases[i], d, n);
        if (x == 1 || x == nm1) continue;
        bool ok = false;
        for (int j = 1; j < s; j++) {
            x = mulmod_seq(x, x, n);
            if (x == nm1) { ok = true; break; }
        }
        if (!ok) return false;
    }
    return true;
}

static seq_t A262298_mult(idx_t k) {
    seq_t m = 10;
    while (k >= 10) { m *= 10; k /= 10; }
    return m;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return 0;
    A262298_init_small_primes();

    uint64_t rem[A262298_NSP];
    for (int i = 0; i < A262298_nsp; i++) rem[i] = 1;

    seq_t r = 1;
    idx_t digits = 1;
    idx_t count = 0;
    for (idx_t k = 3; ; ++k) {
        int d = num_digits_idx(k);
        seq_t mult = A262298_mult(k);
        seq_t t = r * mult;
        if (t / mult != r) return 0;           /* overflow of native seq_t */
        t += (seq_t)k;
        if (t < r) return 0;
        r = t;
        digits += (idx_t)d;

        uint64_t pw = 1;
        for (int j = 0; j < d; j++) pw *= 10;
        for (int i = 0; i < A262298_nsp; i++) {
            uint64_t p = A262298_sp[i];
            rem[i] = ((rem[i] * (pw % p)) % p + k % p) % p;
        }

        /* last digit is k's last digit: must be odd and not 5 */
        idx_t ld = k % 10;
        if (ld % 2 == 0 || ld == 5) continue;

        bool cand;
        if (digits <= 19) {
            cand = is_prime_u64((uint64_t)r);
        } else {
            cand = true;
            for (int i = 0; i < A262298_nsp; i++)
                if (rem[i] == 0) { cand = false; break; }
            if (cand) cand = A262298_mr(r);
        }
        if (cand) {
            count++;
            if (count == n) return r;
        }
    }
}
