/*
 * A316140  Denominator of the autosequence 2/((n+2)*(n+3)) difference table
 *          written by antidiagonals.
 * https://oeis.org/A316140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, tabl, more
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   pow_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A316140 – Denominator of the antidiagonal‑written difference
 *   table of 2/((n+2)(n+3)).
 * The algorithm first converts an index n to its antidiagonal
 *   coordinates (i,j).  The i‑th forward difference Δ^i of the
 *   sequence g(k)=2/((k+2)(k+3)) is
 *     Δ^i g_j = 2*(-1)^i * i! * (i+1) / ((j+2)(j+3+i) * Π_{m=j+3}^{j+2+i} m).
 * The denominator after reduction is obtained by prime factorisation of
 *   the numerator 2·i!·(i+1) and of the denominator above, cancelling
 *   common factors.
 *
 * Note: All arithmetic uses only integer operations suitable for
 *   both narrow (uint8_t…uint64_t) and arbitrary‑precision seq_t types.
 */

#include <stdbool.h>
#include "../utils/pow_small.h"

#define MAXVAL 2000            /* maximal integer that may appear in factorisation */

static bool primes_ready = false;
static size_t prime_count = 0;
static uint32_t primes[400];   /* enough for primes up to 2000 */
static int prime_index[MAXVAL + 1];         /* maps an integer to its prime index, -1 if composite */
static bool is_composite[MAXVAL + 1] = {0}; /* sieve array */

/* Triangular number T(x) = x*(x+1)/2.  Safe for x <= 10^6. */
static inline idx_t triangular(idx_t x)
{
    return x * (x + 1) / 2;
}

/* Initialise primes up to MAXVAL.  Call once per program. */
static void init_primes(void)
{
    if (primes_ready) return;
    for (int i = 0; i <= MAXVAL; ++i) prime_index[i] = -1; /* mark as composite */

    for (int i = 2; i <= MAXVAL; ++i) {
        if (!is_composite[i]) {
            primes[prime_count] = (uint32_t)i;
            prime_index[i] = (int)prime_count;
            ++prime_count;
            if ((long long)i * i <= MAXVAL) {
                for (int j = i * i; j <= MAXVAL; j += i) is_composite[j] = true;
            }
        }
    }
    primes_ready = true;
}

/* Add prime exponents of val to exp_array.  'val' is ≤ MAXVAL. */
static inline void add_factor(idx_t val, int *exp_array)
{
    if (val <= 1) return;
    idx_t tmp = val;
    for (size_t pi = 0; pi < prime_count; ++pi) {
        uint32_t p = primes[pi];
        if ((idx_t)p * p > tmp) break;
        while (tmp % p == 0) {
            ++exp_array[pi];
            tmp /= p;
        }
    }
    if (tmp > 1) {
        int pi = prime_index[(int)tmp];
        if (pi >= 0) ++exp_array[pi];
    }
}

/* Compute the a(n) value. */
seq_t a(idx_t n)
{
    init_primes();

    /* Find s such that T(s) <= n < T(s+1). */
    idx_t s = 0;
    while (triangular(s + 1) <= n) ++s;

    idx_t offset = n - triangular(s);
    idx_t i = offset;          /* difference order */
    idx_t j = s - offset;      /* index in the original sequence */

    /* Exponent arrays for numerator and denominator. */
    static int num_exp[400];
    static int den_exp[400];
    for (size_t p = 0; p < prime_count; ++p) {
        num_exp[p] = 0;
        den_exp[p] = 0;
    }

    /* Numerator: 2 * i! * (i+1). */
    add_factor(2, num_exp);
    for (idx_t k = 1; k <= i; ++k) add_factor(k, num_exp);
    add_factor(i + 1, num_exp);

    /* Denominator: (j+2) * (j+3+i) * Π_{m=j+3}^{j+2+i} m. */
    add_factor(j + 2, den_exp);
    add_factor(j + 3 + i, den_exp);
    for (idx_t k = j + 3; k <= j + 2 + i; ++k) add_factor(k, den_exp);

    /* Cancel common factors and form the reduced denominator. */
    seq_t result = 1;
    for (size_t p = 0; p < prime_count; ++p) {
        int d = den_exp[p];
        int nexp = num_exp[p];
        int rem = d - (d < nexp ? d : nexp);  /* remaining exponent after cancellation */
        if (rem > 0) {
            seq_t pow = pow_small((seq_t)primes[p], (idx_t)rem);
            result = result * pow;
        }
    }
    return result;
}
