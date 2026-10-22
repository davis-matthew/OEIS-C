/*
 * A343022  Discriminants with exactly 1 associated cyclic cubic field.
 * https://oeis.org/A343022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 49, 81 for n = 1, 2; a(n) = p_{n-1}² for n ≥ 3, where p_k
 *            is the k‑th prime ≡ 1 (mod 3)
 * Range:     u8 n <= 3, u16 n <= 26, u32 n <= 3260, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A quick incremental prime generator that keeps track of all primes and
    those ≡ 1 (mod 3).  The generator is used only for n ≥ 3.  */
#include <stdlib.h>
#include <stdint.h>

/* Prime storage.  all_primes holds every prime found (for trial division),
   primes1mod3 holds those with p ≡ 1 (mod 3).  */
static uint64_t *all_primes = NULL;
static size_t    all_primes_cnt = 0;
static uint64_t *primes1mod3   = NULL;
static size_t    primes1mod3_cnt = 0;

/* The last integer that has been tested for primality. */
static uint64_t last_checked = 1;

/* Ensure that primes1mod3 has at least target_index+1 elements (0‑based). */
static void
ensure_primes1mod3(idx_t target_index)
{
    /* Grow until we have enough 1‑mod‑3 primes. */
    while (primes1mod3_cnt <= (size_t)target_index) {
        uint64_t candidate = last_checked + 1;
        int is_prime = 1;

        /* Test divisibility by previously found primes. */
        for (size_t i = 0; i < all_primes_cnt; ++i) {
            uint64_t p = all_primes[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            /* Add to all_primes array, resizing if required. */
            if (all_primes_cnt == 0 || all_primes_cnt % 64 == 0) {
                uint64_t *tmp = (uint64_t *)realloc(all_primes,
                        sizeof(uint64_t) * (all_primes_cnt + 64));
                if (!tmp) exit(1);
                all_primes = tmp;
            }
            all_primes[all_primes_cnt++] = candidate;

            /* If prime ≡ 1 (mod 3), add to the special list. */
            if (candidate % 3ULL == 1) {
                if (primes1mod3_cnt == 0 || primes1mod3_cnt % 32 == 0) {
                    uint64_t *tmp = (uint64_t *)realloc(primes1mod3,
                            sizeof(uint64_t) * (primes1mod3_cnt + 32));
                    if (!tmp) exit(1);
                    primes1mod3 = tmp;
                }
                primes1mod3[primes1mod3_cnt++] = candidate;
            }
        }

        last_checked = candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)49;   /* 7² */
    if (n == 2) return (seq_t)81;   /* 9² */

    /* For n ≥ 3: use the (n‑1)‑th prime ≡ 1 (mod 3).  The primes1mod3
       array is 0‑based, with index 0 = 7.  Hence index = (n‑1)‑1 = n‑2. */
    idx_t target = n - 2;
    ensure_primes1mod3(target);

    uint64_t k = primes1mod3[(size_t)target];
    return (seq_t)k * (seq_t)k;
}
