/*
 * A335402  Numbers m such that the only normal integer partition of m whose
 *          run-lengths are a palindrome is (1)^m.
 * https://oeis.org/A335402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A335402: numbers m such that the only normal integer partition of m whose run‑lengths are a palindrome is (1)^m.
   Sequence values: a(1)=0, a(2)=1, a(3)=2, a(4)=4, and for n≥5, a(n) is the (n‑4)‑th prime > 3. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static uint64_t *prime_arr = NULL;
static size_t prime_cnt   = 0;
static size_t prime_cap   = 0;
static bool   prime_inited = false;

/* Generate enough primes to have at least 'need' primes in prime_arr (1‑based index). */
static void ensure_primes(size_t need)
{
    if (prime_cnt >= need) return;
    if (!prime_inited) {
        /* initialise with small primes 2,3 */
        prime_cap = 16;
        prime_arr = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
        prime_arr[0] = 2;
        prime_arr[1] = 3;
        prime_cnt = 2;
        prime_inited = true;
    }
    uint64_t cand = prime_arr[prime_cnt - 1] + 1;
    while (prime_cnt < need) {
        bool is_prime = true;
        uint64_t r = (uint64_t)sqrt((double)cand);
        for (size_t i = 0; i < prime_cnt && prime_arr[i] <= r; ++i) {
            if (cand % prime_arr[i] == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                prime_arr = (uint64_t*)realloc(prime_arr, prime_cap * sizeof(uint64_t));
            }
            prime_arr[prime_cnt++] = cand;
        }
        ++cand;
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;
    if (n == 4) return (seq_t)4;
    /* n ≥ 5: return the (n-4)-th prime > 3, i.e. the (n-4+2)-th prime overall */
    idx_t prime_index = (idx_t)(n - 4 + 2);   /* 1‑based */
    ensure_primes((size_t)prime_index);
    return (seq_t)prime_arr[prime_index - 1];
}
