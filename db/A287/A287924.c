/*
 * A287924  Numbers k such that A287922(k) is a prime.
 * https://oeis.org/A287924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th k such that prime(k)^2 + 4 is prime
 *            (A287922(k) = 4 + prime(k)^2)
 * Range:     u8 n <= 52, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-08)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

static uint32_t *A287924_pr = NULL;
static size_t A287924_np = 0, A287924_pcap = 0;
static uint64_t *A287924_t = NULL;
static size_t A287924_nt = 0, A287924_tcap = 0;

static uint32_t A287924_next_prime(void)
{
    if (A287924_np == A287924_pcap) {
        A287924_pcap = A287924_pcap ? A287924_pcap * 2 : 64;
        A287924_pr = (uint32_t *)realloc(A287924_pr, A287924_pcap * sizeof(uint32_t));
        if (!A287924_pr) abort();
    }
    uint32_t p;
    if (A287924_np == 0) {
        p = 2;
    } else {
        p = A287924_pr[A287924_np - 1];
        p = (p == 2) ? 3 : p + 2;
        for (;; p += 2) {
            bool ok = true;
            for (size_t i = 0; i < A287924_np; ++i) {
                uint64_t q = A287924_pr[i];
                if (q * q > p) break;
                if (p % q == 0) { ok = false; break; }
            }
            if (ok) break;
        }
    }
    A287924_pr[A287924_np++] = p;
    return p;
}

seq_t a(idx_t n)
{
    while (A287924_nt < (size_t)n) {
        uint32_t p = A287924_next_prime();
        uint64_t v = (uint64_t)p * p + 4;
        if (is_prime_u64(v)) {
            if (A287924_nt == A287924_tcap) {
                A287924_tcap = A287924_tcap ? A287924_tcap * 2 : 64;
                A287924_t = (uint64_t *)realloc(A287924_t, A287924_tcap * sizeof(uint64_t));
                if (!A287924_t) abort();
            }
            A287924_t[A287924_nt++] = (uint64_t)A287924_np;
        }
    }
    return (seq_t)A287924_t[(size_t)n - 1];
}
