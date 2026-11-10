/*
 * A270617  Primes p such that A256832(p) is divisible by p.
 * https://oeis.org/A270617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime p such that A256832(p) = Product_{k=1..p}
 *            Pell(k) == 0 (mod p); search over primes: p=2, or p ≡ ±1 (mod 8)
 *            (rank of apparition divides p-1), or else p ≡ ±3 (mod 8) and
 *            some prime q | p+1 has Pell((p+1)/q) ≡ 0 (mod p)
 * Range:     u8 n <= 43, u16 n <= 5320, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9356 (2026-11-07)
 * Helpers:   is_prime, mem, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/mulmod.h"
#include "../utils/mem.h"

static uint64_t A270617_pell_mod(uint64_t k, uint64_t m)
{
    uint64_t b00 = 2 % m, b01 = 1 % m, b10 = 1 % m, b11 = 0;
    uint64_t r00 = 1 % m, r01 = 0, r10 = 0, r11 = 1 % m;
    while (k > 0) {
        if (k & 1ULL) {
            uint64_t t00 = (mulmod(r00, b00, m) + mulmod(r01, b10, m)) % m;
            uint64_t t01 = (mulmod(r00, b01, m) + mulmod(r01, b11, m)) % m;
            uint64_t t10 = (mulmod(r10, b00, m) + mulmod(r11, b10, m)) % m;
            uint64_t t11 = (mulmod(r10, b01, m) + mulmod(r11, b11, m)) % m;
            r00 = t00; r01 = t01; r10 = t10; r11 = t11;
        }
        uint64_t s00 = (mulmod(b00, b00, m) + mulmod(b01, b10, m)) % m;
        uint64_t s01 = (mulmod(b00, b01, m) + mulmod(b01, b11, m)) % m;
        uint64_t s10 = (mulmod(b10, b00, m) + mulmod(b11, b10, m)) % m;
        uint64_t s11 = (mulmod(b10, b01, m) + mulmod(b11, b11, m)) % m;
        b00 = s00; b01 = s01; b10 = s10; b11 = s11;
        k >>= 1;
    }
    return r01;
}

static bool A270617_ok(uint64_t p)
{
    if (p == 2) return true;
    uint64_t r = p % 8;
    if (r == 1 || r == 7) return true;
    uint64_t np1 = p + 1;
    uint64_t t = np1;
    for (uint64_t q = 2; q * q <= t; q++) {
        if (t % q == 0) {
            if (A270617_pell_mod(np1 / q, p) == 0) return true;
            while (t % q == 0) t /= q;
        }
    }
    if (t > 1) {
        if (A270617_pell_mod(np1 / t, p) == 0) return true;
    }
    return false;
}

static seq_t *A270617_terms = NULL;
static size_t A270617_len = 0, A270617_cap = 0;
static uint64_t A270617_next = 2;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A270617_len < (size_t)n) {
        uint64_t p = A270617_next++;
        if (!is_prime((idx_t)p)) continue;
        if (!A270617_ok(p)) continue;
        if (A270617_len >= A270617_cap) {
            size_t nc = A270617_cap ? A270617_cap * 2 : 1024;
            seq_t *np = mem_resize(seq_t, A270617_terms, nc);
            A270617_terms = np;
            A270617_cap = nc;
        }
        A270617_terms[A270617_len++] = (seq_t)p;
    }
    return A270617_terms[(size_t)n - 1];
}
