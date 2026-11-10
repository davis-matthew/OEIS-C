/*
 * A366806  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A324186(i) = A324186(j) for all i, j >= 0, where A324186 is the
 *          sum of odd divisors permuted by A163511.
 * https://oeis.org/A366806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence of A324186, where A324186(n) =
 *            A000593(A163511(n)) (sum of odd divisors of A163511(n));
 *            computed by first-occurrence ranking of the keys
 * Range:     u8 n <= 552, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 105 terms through n = 9379 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

typedef unsigned __int128 a366806_key;

static a366806_key *a366806_keys = 0;   /* distinct keys in order of first occurrence */
static size_t a366806_nkeys = 0, a366806_capk = 0;
static uint32_t *a366806_rg = 0;        /* rg[k] = value for index k */
static size_t a366806_done = 0, a366806_capr = 0;

static uint32_t a366806_prime(int i) /* i-th prime, i>=1 */
{
    uint32_t c = 1;
    int cnt = 0;
    while (cnt < i) {
        c++;
        int ok = 1;
        for (uint32_t d = 2; d * d <= c; d++)
            if (c % d == 0) { ok = 0; break; }
        if (ok) cnt++;
    }
    return c;
}

static a366806_key a366806_key_of(uint64_t k)
{
    if (k == 0) return 1;
    int pos[64];
    int r = 0, bp = 1;
    uint64_t t = k;
    while (t) {
        if (t & 1) pos[r++] = bp;
        bp++;
        t >>= 1;
    }
    a366806_key res = 1;
    int prev = 0;
    for (int i = 0; i < r; i++) {
        int e = pos[i] - prev - 1;
        prev = pos[i];
        if (i == r - 1) e++;
        if (i == 0) continue; /* prime 2 dropped */
        a366806_key p = (a366806_key)a366806_prime(i + 1);
        a366806_key s = 1, pw = 1;
        for (int j = 0; j < e; j++) {
            pw *= p;
            s += pw;
        }
        res *= s;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n >= a366806_done) {
        size_t need = (size_t)n + 1;
        if (need > a366806_capr) {
            size_t nc = need * 2;
            a366806_rg = (uint32_t *)realloc(a366806_rg, nc * sizeof(uint32_t));
            a366806_capr = nc;
        }
        for (size_t k = a366806_done; k < need; k++) {
            a366806_key key = a366806_key_of((uint64_t)k);
            size_t j;
            for (j = 0; j < a366806_nkeys; j++)
                if (a366806_keys[j] == key) break;
            if (j == a366806_nkeys) {
                if (a366806_nkeys == a366806_capk) {
                    a366806_capk = a366806_capk ? a366806_capk * 2 : 1024;
                    a366806_keys = (a366806_key *)realloc(a366806_keys, a366806_capk * sizeof(a366806_key));
                }
                a366806_keys[a366806_nkeys++] = key;
            }
            a366806_rg[k] = (uint32_t)(j + 1);
        }
        a366806_done = need;
    }
    return (seq_t)a366806_rg[n];
}
