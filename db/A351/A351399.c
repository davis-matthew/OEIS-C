/*
 * A351399  Sum of the exponents in the prime factorizations of the divisors
 *          of A100716(n) of the form p^p, p prime.
 * https://oeis.org/A351399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{prime p, p^p | A100716(n)} p, i.e. b(A100716(n))
 *            with b additive, b(p^e) = p if p <= e else 0; computed by a
 *            linear scan of k
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 110 terms through n = 8966 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *a351399_buf = NULL;
static idx_t  a351399_len = 0;
static idx_t  a351399_cap = 0;
static seq_t  a351399_k = 1;

/* sum of primes p with p^p dividing k (0 if none) */
static seq_t a351399_b(seq_t k)
{
    seq_t sum = 0;
    for (unsigned long long p = 2; ; ++p) {
        bool pr = true;
        for (unsigned long long d = 2; d * d <= p; ++d) {
            if (p % d == 0) { pr = false; break; }
        }
        if (!pr) continue;
        seq_t pw = 1;
        bool big = false;
        for (unsigned long long i = 0; i < p; ++i) {
            if (pw > k / (seq_t)p) { big = true; break; }
            pw *= (seq_t)p;
        }
        if (big) break;
        if (k % pw == 0) sum += (seq_t)p;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (a351399_len < n) {
        ++a351399_k;
        seq_t s = a351399_b(a351399_k);
        if (s > 0) {
            if (a351399_len == a351399_cap) {
                a351399_cap = a351399_cap ? a351399_cap * 2 : 64;
                a351399_buf = mem_resize(seq_t, a351399_buf, a351399_cap);
                if (!a351399_buf) abort();
            }
            a351399_buf[a351399_len++] = s;
        }
    }
    return a351399_buf[n - 1];
}
