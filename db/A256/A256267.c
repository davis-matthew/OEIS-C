/*
 * A256267  a(n) = A256017(n)/n.
 * https://oeis.org/A256267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 251, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the least integer t>1 such that
 * all divisors of k = n*t that are ≤ n also divide n.
 * Then a(n)=t.  This t is equal to A256017(n)/n.  */
#include <stdint.h>
#include <stdbool.h>

/* Return all divisors of num into out (max 2000 entries),
 * set *cnt to number of divisors (unsorted). */
static void get_divisors(idx_t num, idx_t out[], int *cnt)
{
    idx_t temp = num;
    idx_t primes[64];
    int exps[64];
    int pc = 0;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pc] = p;
            exps[pc] = e;
            ++pc;
        }
    }
    if (temp > 1) {
        primes[pc] = temp;
        exps[pc] = 1;
        ++pc;
    }

    out[0] = 1;
    int dcnt = 1;
    for (int i = 0; i < pc; ++i) {
        int old = dcnt;
        idx_t p = primes[i];
        idx_t pw = 1;
        for (int e = 1; e <= exps[i]; ++e) {
            pw *= p;
            for (int j = 0; j < old; ++j) {
                out[dcnt++] = out[j] * pw;
            }
        }
    }
    *cnt = dcnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t divn[2048];
    int dcntn = 0;
    get_divisors(n, divn, &dcntn);

    for (idx_t t = 2; ; ++t) {
        idx_t divt[2048];
        int dcntt = 0;
        get_divisors(t, divt, &dcntt);

        bool ok = true;
        for (int i = 0; i < dcntn && ok; ++i) {
            idx_t d1 = divn[i];
            for (int j = 0; j < dcntt; ++j) {
                idx_t d2 = divt[j];
                unsigned __int128 prod = (unsigned __int128)d1 * d2;
                if (prod <= n) {
                    if (n % (idx_t)prod != 0) {
                        ok = false;
                        break;
                    }
                }
            }
        }
        if (ok) {
            return (seq_t)t;
        }
    }
}
