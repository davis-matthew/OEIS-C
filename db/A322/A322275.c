/*
 * A322275  Smallest multiplication factors f, prime or 1, for all b (mod
 *          120120), coprime to 120120 (= 4*13#), so that b*f is a square mod
 *          8, and modulo all primes up to 13.
 * https://oeis.org/A322275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = smallest f∈{1}∪{prime}>13 such that * b(n)*f is a
 *            quadratic residue modulo all primes ≤13 * and modulo 8, where
 *            b(n) is the n‐th integer coprime to 120120 * The sequence is
 *            periodic with period φ(120120)=23040. * This implementation
 *            precomputes b(n) and evaluates a(n) once per residue * an...
 * Range:     u8 n <= 49, u16 n <= 9658, u32 n <= 9658, u64 n <= 9658,
 *            u128 n <= 9658, big any n
 * Helpers:   gcd_u64, modpow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"
#include "../utils/modpow_int.h"

/* gcd for 64 bit unsigned integers */

/* prime list up to 4000 (skipping 2,3,5,7,11,13) */
static seq_t *prime_list = NULL;
static size_t prime_cnt = 0;

static void init_primes(void)
{
    const int LIM = 4000;
    bool *is_prime = (bool *)calloc(LIM + 1, sizeof(bool));
    if (!is_prime) exit(EXIT_FAILURE);
    for (int i = 2; i <= LIM; ++i) is_prime[i] = true;
    for (int i = 2; i * i <= LIM; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= LIM; j += i) is_prime[j] = false;
        }
    }
    /* count primes >13 */
    size_t cnt = 0;
    for (int i = 17; i <= LIM; ++i) if (is_prime[i]) ++cnt;
    prime_list = (seq_t *)malloc(cnt * sizeof(seq_t));
    if (!prime_list) exit(EXIT_FAILURE);
    size_t idx = 0;
    for (int i = 17; i <= LIM; ++i) if (is_prime[i]) prime_list[idx++] = (seq_t)i;
    prime_cnt = cnt;
    free(is_prime);
}

/* Legendre symbol tables for primes 3,5,7,11,13 */
static const int primes_q[5] = {3, 5, 7, 11, 13};
static int legendre_res[5][14];   /* second dimension covers max prime 13 */

/* initialize legendre_res */
static void init_legendre_res(void)
{
    for (int idx = 0; idx < 5; ++idx) {
        int q = primes_q[idx];
        for (int r = 0; r < q; ++r) {
            if (r == 0) legendre_res[idx][r] = 0;
            else {
                int val = modpow_int(r, (q - 1) / 2, q);
                legendre_res[idx][r] = (val == 1) ? 1 : -1;
            }
        }
    }
}

/* global data for memoization */
static bool initialized = false;
static seq_t *b_list = NULL, *f_list = NULL;
static size_t period = 0;

/* Initialization routine computing all b(n) and a(n) up to one period */
static void init_sequence(void)
{
    /* ---------- compute list of numbers coprime to 120120 ---------- */
    const uint64_t MOD = 120120ull;
    size_t cnt = 0;
    for (uint64_t i = 1; i <= MOD; ++i)
        if (gcd_u64(i, MOD) == 1) ++cnt;
    period = cnt; /* φ(120120) = 23040 */
    b_list = (seq_t *)malloc(period * sizeof(seq_t));
    f_list = (seq_t *)malloc(period * sizeof(seq_t));
    if (!b_list || !f_list) exit(EXIT_FAILURE);

    size_t pos = 0;
    for (uint64_t i = 1; i <= MOD; ++i) {
        if (gcd_u64(i, MOD) == 1) {
            b_list[pos] = (seq_t)i;
            ++pos;
        }
    }

    /* ---------- prepare primes and legendre tables ---------- */
    init_primes();
    init_legendre_res();

    /* ---------- compute a(n) for each residue ---------- */
    for (size_t idx = 0; idx < period; ++idx) {
        seq_t b = b_list[idx];
        /* compute Legendre sign for each prime q */
        int signs[5];
        for (int iq = 0; iq < 5; ++iq) {
            int q = primes_q[iq];
            int r = (int)(b % (seq_t)q);
            signs[iq] = legendre_res[iq][r];
        }
        /* check if f = 1 is suitable */
        bool f1_ok = true;
        if ((b % (seq_t)8) != (seq_t)1) f1_ok = false;
        for (int iq = 0; iq < 5; ++iq)
            if (signs[iq] != 1) { f1_ok = false; break; }

        if (f1_ok) {
            f_list[idx] = (seq_t)1;
            continue;
        }

        /* search for smallest prime that matches all conditions */
        seq_t chosen = (seq_t)0;
        uint8_t b_mod8 = (uint8_t)(b % (seq_t)8);
        for (size_t pidx = 0; pidx < prime_cnt; ++pidx) {
            seq_t p = prime_list[pidx];
            if ((uint8_t)(p % (seq_t)8) != b_mod8) continue;
            bool ok = true;
            for (int iq = 0; iq < 5; ++iq) {
                int q = primes_q[iq];
                int r = (int)(p % (seq_t)q);
                if (legendre_res[iq][r] != signs[iq]) { ok = false; break; }
            }
            if (ok) {
                chosen = p;
                break;
            }
        }
        if (chosen == 0) {
            /* Fallback: theoretically impossible with known max 3583 */
            chosen = (seq_t)1;
        }
        f_list[idx] = chosen;
    }

    initialized = true;
}

/* The required sequence function */
seq_t a(idx_t n)
{
    if (!initialized) init_sequence();

    /* sequence is periodic with period = φ(120120) */
    idx_t idx = (n - 1) % (idx_t)period;
    return f_list[(size_t)idx];
}
