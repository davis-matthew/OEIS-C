/*
 * A292498  G.f.: Sum_{n=-oo..+oo} Fibonacci(n+1) * x^n * (1-x^n)^n.
 * https://oeis.org/A292498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A292498: a(n) = sum_{d|n} {
 *   (t <= d+1 ? Fibonacci(d+1) * (-1)^{t-1} * C(d, t-1) : 0)
 *   + (t >= d-1 ? Fibonacci(d-1) * C(t, d-1) : 0)
 * } where t = n/d, C is the binomial coefficient.
 * a(0) = 1.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Fibonacci numbers up to required index, cached. */
static seq_t get_fib(idx_t n) {
#ifdef __cplusplus
    static std::vector<seq_t> fib;
    if (fib.size() < 2) {
        fib.resize(2);
        fib[0] = 0;
        fib[1] = 1;
    }
    if ((size_t)n >= fib.size()) {
        size_t old = fib.size();
        fib.resize(n + 1);
        for (size_t i = old; i <= (size_t)n; ++i) {
            fib[i] = fib[i - 1] + fib[i - 2];
        }
    }
    return fib[(size_t)n];
#else
    static seq_t *fib = NULL;
    static size_t fib_size = 0;
    if (fib_size == 0) {
        fib_size = 2;
        fib = (seq_t *)malloc(fib_size * sizeof(seq_t));
        fib[0] = 0;
        fib[1] = 1;
    }
    if ((size_t)n >= fib_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(fib, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0; /* allocation failure */
        for (size_t i = fib_size; i <= (size_t)n; ++i) {
            tmp[i] = tmp[i - 1] + tmp[i - 2];
        }
        fib = tmp;
        fib_size = new_size;
    }
    return fib[(size_t)n];
#endif
}

/* multiplicative binomial coefficient C(a,b). */
static seq_t binom_small(idx_t a, idx_t b) {
    if (b > a) return 0;
    if (b > a - b) b = a - b;
    seq_t res = 1;
    for (idx_t i = 1; i <= b; ++i) {
        res *= (seq_t)(a - b + i);
        res /= (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t pos = 0;
    seq_t neg = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0) continue;
        idx_t t1 = n / d;
        /* first divisor d */
        {
            idx_t dval = d;
            idx_t t = t1;
            if (t <= dval + 1) {
                seq_t val = get_fib(dval + 1) * binom_small(dval, t - 1);
                if ((t - 1) & 1) {
                    neg += val;
                } else {
                    pos += val;
                }
            }
            if (t >= dval - 1) {
                seq_t val = get_fib(dval - 1) * binom_small(t, dval - 1);
                pos += val;
            }
        }
        if (d != t1) {
            idx_t dval = t1;
            idx_t t = d;
            if (t <= dval + 1) {
                seq_t val = get_fib(dval + 1) * binom_small(dval, t - 1);
                if ((t - 1) & 1) {
                    neg += val;
                } else {
                    pos += val;
                }
            }
            if (t >= dval - 1) {
                seq_t val = get_fib(dval - 1) * binom_small(t, dval - 1);
                pos += val;
            }
        }
    }
    return pos - neg;
}
