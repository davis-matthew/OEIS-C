/*
 * A368085  Square array read by ascending antidiagonals: row n is the
 *          trajectory of P under the 'Px+1' map, where P = n-th prime.
 * https://oeis.org/A368085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is obtained by reading the square array of trajectories
 *            defined above along ascending anti‑diagonals. The value is the
 *            j‑th element of the trajectory of the i‑th prime, where (i,j) is
 *            the position in the array corresponding to the index n
 * Range:     u8 n <= 29, u16 n <= 135, u32 n <= 527, u64 n <= 2079,
 *            u128 n <= 8255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* --------------------------------------------------------------------- */
/*  Prime utility – dynamically generates primes as needed                */
/* --------------------------------------------------------------------- */

static uint32_t *prime_list = NULL;   /* array of generated primes      */
static size_t    prime_count = 0;     /* number of primes stored        */

/* Add the next prime to prime_list.  Returns the prime added. */
static uint32_t add_next_prime(void)
{
    uint32_t candidate = (prime_count == 0) ? 2 : prime_list[prime_count - 1] + 1;
    for (;;) {
        bool prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * (uint64_t)p > candidate) break;
            if (candidate % p == 0) { prime = false; break; }
        }
        if (prime) {
            prime_list = (uint32_t *)realloc(prime_list,
                                            (prime_count + 1) * sizeof(uint32_t));
            prime_list[prime_count++] = candidate;
            return candidate;
        }
        ++candidate;
    }
}

/* Return the k‑th prime (1‑based).  Extends prime_list as necessary. */
static uint32_t get_prime_k(size_t k)
{
    while (prime_count < k)
        add_next_prime();
    return prime_list[k - 1];
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                               */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1 */

    /* Find diagonal s such that T(s-1) < n ≤ T(s), where T(s)=s(s+1)/2. */
    idx_t s = 0;
    idx_t t = 0;
    while (t < n) {
        ++s;
        t += s;               /* now t == T(s) */
    }
    idx_t prev_t = t - s;   /* T(s-1)   */
    idx_t pos    = n - prev_t;  /* position inside the diagonal (1‑based) */

    /* Determine row (i) and column (j).  j equals pos. */
    idx_t row = s - pos + 1;    /* 1‑based row index */
    idx_t col = pos;            /* 1‑based column index */

    /* Starting value: the i‑th prime. */
    seq_t P = (seq_t)get_prime_k((size_t)row);

    seq_t x = P;

    /* Apply the Px+1 map (col‑1) times. */
    for (idx_t step = 1; step < col; ++step) {
        /* Find smallest prime < P that divides x. */
        bool divided = false;
        for (size_t pi = 0; pi < prime_count; ++pi) {
            uint32_t p = prime_list[pi];
            if (p >= (uint32_t)P) break;            /* only primes < P matter */
            if ((x % (seq_t)p) == 0) {
                x = x / (seq_t)p;
                divided = true;
                break;
            }
        }
        if (!divided)
            x = (x * P) + 1;
    }

    return x;
}
