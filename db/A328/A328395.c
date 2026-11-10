/*
 * A328395  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A046523(A276087(i)) = A046523(A276087(j)) for all i, j.
 * https://oeis.org/A328395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = RGS transform of f(n) = A046523(A276086(A276086(n)));
 *            computed by incremental first-appearance indexing of prime
 *            signatures (primorial-base digits of A276086(n))
 * Range:     u8 n <= 16, u16 n <= 86, u32 n <= 1351, u64 n <= 32589,
 *            u128 n <= 32589, big any n
 * Validated: standard, pass, 118 terms through n = 9903 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

#define A328395_NP 64
#define A328395_KL 64

static unsigned A328395_pr[A328395_NP];

static void A328395_init_primes(void)
{
    static bool done = false;
    if (done) return;
    unsigned cnt = 0, c = 2;
    while (cnt < A328395_NP) {
        bool ok = true;
        for (unsigned i = 0; i < cnt; i++) {
            if (A328395_pr[i] * A328395_pr[i] > c) break;
            if (c % A328395_pr[i] == 0) { ok = false; break; }
        }
        if (ok) A328395_pr[cnt++] = c;
        c++;
    }
    done = true;
}

/* A276086(n) as seq_t */
static seq_t A328395_inner(idx_t n)
{
    seq_t m = 1;
    int k = 0;
    while (n > 0 && k < A328395_NP) {
        unsigned p = A328395_pr[k];
        unsigned d = (unsigned)(n % p);
        n /= p;
        for (unsigned j = 0; j < d; j++) m = m * (seq_t)p;
        k++;
    }
    return m;
}

typedef struct {
    unsigned char len;
    unsigned char d[A328395_KL];
} A328395_key;

static A328395_key *A328395_keys = NULL;
static size_t A328395_nkeys = 0, A328395_kcap = 0;
static uint32_t *A328395_res = NULL;
static size_t A328395_nres = 0, A328395_rcap = 0;

static void A328395_make_key(idx_t n, A328395_key *key)
{
    seq_t x = A328395_inner(n);
    key->len = 0;
    int k = 0;
    while (x > 0 && k < A328395_NP) {
        unsigned p = A328395_pr[k];
        unsigned d = (unsigned)(long long)(x % (seq_t)p);
        x = x / (seq_t)p;
        k++;
        if (d == 0) continue;
        /* insertion, descending order */
        int pos = key->len;
        if (pos >= A328395_KL) break;
        while (pos > 0 && key->d[pos - 1] < d) {
            key->d[pos] = key->d[pos - 1];
            pos--;
        }
        key->d[pos] = (unsigned char)d;
        key->len++;
    }
}

seq_t a(idx_t n)
{
    A328395_init_primes();
    while (A328395_nres <= (size_t)n) {
        idx_t i = (idx_t)A328395_nres;
        A328395_key key;
        memset(&key, 0, sizeof key);
        A328395_make_key(i, &key);
        size_t found = A328395_nkeys;
        for (size_t j = 0; j < A328395_nkeys; j++) {
            if (A328395_keys[j].len == key.len &&
                memcmp(A328395_keys[j].d, key.d, key.len) == 0) { found = j; break; }
        }
        if (found == A328395_nkeys) {
            if (A328395_nkeys == A328395_kcap) {
                A328395_kcap = A328395_kcap ? A328395_kcap * 2 : 64;
                A328395_keys = (A328395_key *)realloc(A328395_keys, A328395_kcap * sizeof(A328395_key));
                if (!A328395_keys) abort();
            }
            A328395_keys[A328395_nkeys++] = key;
        }
        if (A328395_nres == A328395_rcap) {
            A328395_rcap = A328395_rcap ? A328395_rcap * 2 : 1024;
            A328395_res = (uint32_t *)realloc(A328395_res, A328395_rcap * sizeof(uint32_t));
            if (!A328395_res) abort();
        }
        A328395_res[A328395_nres++] = (uint32_t)(found + 1);
    }
    return (seq_t)A328395_res[(size_t)n];
}
