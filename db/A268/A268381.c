/*
 * A268381  Numbers having at least the same number of prime factors of the
 *          form 4*k+1 than of the form 4*k+3, when counted with multiplicity.
 * https://oeis.org/A268381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k ≥ 1 satisfying (# of prime factors of k
 *            ≡ 1 (mod 4)) ≥ (# of prime factors of k ≡ 3 (mod 4)), counted
 *            with multiplicity. No closed form; the code builds the sequence
 *            incrementally using a smallest‑prime‑factor sieve
 * Range:     u8 n <= 123, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/*  Cache for the generated sequence terms                            */
/* ------------------------------------------------------------------ */
static seq_t *seq_arr = NULL;   /* dynamic array of computed terms          */
static size_t seq_len = 0;      /* number of terms currently cached         */
static size_t seq_cap = 0;      /* allocated capacity of seq_arr             */

/* Ensure seq_arr has enough space to append one more term.         */
static void grow_seq(void)
{
    if (seq_cap == 0) {
        seq_cap = 128;
        seq_arr = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    } else {
        seq_cap <<= 1;                     /* double the capacity          */
        seq_arr = (seq_t*)realloc(seq_arr, seq_cap * sizeof(seq_t));
    }
}

/* ------------------------------------------------------------------ */
/*  Smallest‑prime‑factor sieve                                       */
/* ------------------------------------------------------------------ */
static uint64_t *spf = NULL;   /* array spf[i] = smallest prime factor of i  */
static size_t spf_lim = 0;     /* current upper bound of the sieve           */

/* Grow the sieve array to cover all numbers ≤ n.                     */
static void ensure_spf(size_t n)
{
    if (n <= spf_lim) return;

    size_t new_lim = spf_lim ? spf_lim * 2 : 1;
    if (new_lim < n) new_lim = n;

    uint64_t *new_spf = (uint64_t*)malloc((new_lim + 1) * sizeof(uint64_t));
    for (size_t i = 0; i <= new_lim; ++i) new_spf[i] = i;

    for (size_t i = 2; i * i <= new_lim; ++i) {
        if (new_spf[i] == i) {                     /* prime */
            for (size_t j = i * i; j <= new_lim; j += i) {
                if (new_spf[j] == j) new_spf[j] = i;
            }
        }
    }

    free(spf);
    spf = new_spf;
    spf_lim = new_lim;
}

/* Retrieve the smallest prime factor of a number ≤ spf_lim.          */
static unsigned int spf_of(uint64_t x) { return (unsigned int)spf[x]; }

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined for n = 0 */

    size_t target = (size_t)n;
    static uint64_t current = 1;               /* next integer to test */

    while (seq_len < target) {
        ensure_spf(current);

        size_t c1 = 0, c3 = 0;                 /* counters for 1‑mod‑4 and 3‑mod‑4 primes */
        uint64_t m = current;

        while (m > 1) {
            unsigned int p = spf_of(m);
            do {
                m /= p;
                if ((p & 3ULL) == 1) ++c1;
                else if ((p & 3ULL) == 3) ++c3;
            } while (m % p == 0);
        }

        if (c1 >= c3) {                       /* qualifies for the sequence */
            if (seq_len >= seq_cap) grow_seq();
            seq_arr[seq_len++] = (seq_t)current;
        }

        ++current;
    }

    return seq_arr[target - 1];
}
