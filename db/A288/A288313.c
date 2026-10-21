/*
 * A288313  Let b(k) denote A056240(k); the sequence lists numbers b(2*n)
 *          where for all m > n, b(2*m) > b(2*n).
 * https://oeis.org/A288313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 2451, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A288313
   a(1)=2, a(2)=4, a(3)=8,
   a(n)=3*prime(n-1) for n≥4
   where prime(k) is the k‑th prime. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static uint64_t *prime_buf = NULL;   /* array of primes */
static size_t prime_cnt = 0;         /* number of primes stored */

/* Generate all primes up to a fixed limit (200000 guarantees >=10000 primes). */
static void generate_primes(void)
{
    const size_t LIMIT = 200000;          /* safe upper bound for 10000th prime */
    bool *is_comp = (bool*)calloc(LIMIT + 1, sizeof(bool));
    if (!is_comp)
        exit(1);                         /* allocation error */
    prime_buf = NULL;
    prime_cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            /* store prime */
            size_t new_cnt = prime_cnt + 1;
            uint64_t *new_buf = (uint64_t*)realloc(prime_buf, new_cnt * sizeof(uint64_t));
            if (!new_buf) {
                free(is_comp);
                exit(1);
            }
            prime_buf = new_buf;
            prime_buf[prime_cnt++] = (uint64_t)i;
            /* mark composites */
            if (i <= LIMIT / i) {
                for (size_t j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
}

/* Return the nth value of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)8;

    /* ensure primes are generated once */
    if (prime_cnt == 0)
        generate_primes();

    /* n≥4: use 3*prime(n-1) */
    size_t idx = (size_t)(n - 2);   /* prime index, 0‑based */
    if (idx >= prime_cnt)
        exit(1);                    /* n too large for precomputed primes */

    seq_t result = (seq_t)3 * (seq_t)prime_buf[idx];
    return result;
}
