/*
 * A360543  a(n) = number of numbers k < n, gcd(k, n) > 1, such that omega(k)
 *          > omega(n) and rad(n) | rad(k), where omega(n) = A001221(n) and
 *          rad(n) = A007947(n).
 * https://oeis.org/A360543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ k < n : gcd(k,n) > 1 && ω(k) > ω(n) && rad(n) | rad(k)
 *            }
 * Range:     u8 n <= 1023, u16 n <= 8002, u32 n <= 8002, u64 n <= 8002,
 *            u128 n <= 8002, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

/* Static cache for sieve information */
static idx_t  cache_size = 0;   /* maximum n processed so far (+1)          */
static uint64_t *spf   = NULL; /* smallest prime factor array              */
static uint64_t *rad_arr   = NULL; /* rad(n) array                            */
static uint64_t *omega_arr = NULL; /* ω(n) array                              */

/* Ensure the cache covers all numbers up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;           /* already covered */

    size_t new_size = (size_t)n + 1;       /* array size (0 .. n) */

    /* Reallocate arrays */
    spf   = (uint64_t *)realloc(spf,   new_size * sizeof(uint64_t));
    rad_arr   = (uint64_t *)realloc(rad_arr, new_size * sizeof(uint64_t));
    omega_arr = (uint64_t *)realloc(omega_arr, new_size * sizeof(uint64_t));

    if (!spf || !rad_arr || !omega_arr) {  /* allocation failure */
        abort();
    }

    /* ---- Compute smallest prime factor array (sieve) ---- */
    for (size_t i = 0; i < new_size; ++i) spf[i] = 0;
    if (new_size > 0) spf[0] = 0;
    if (new_size > 1) spf[1] = 1;
    for (size_t i = 2; i < new_size; ++i) {
        if (spf[i] == 0) {                     /* i is prime */
            spf[i] = i;
            if (i * i < new_size) {
                for (size_t j = i * i; j < new_size; j += i) {
                    if (spf[j] == 0) spf[j] = i;
                }
            }
        }
    }

    /* ---- Compute rad(n) and ω(n) using the spf array ---- */
    rad_arr[0]   = 1;
    omega_arr[0] = 0;
    if (new_size > 1) {
        rad_arr[1]   = 1;
        omega_arr[1] = 0;
    }

    for (size_t i = 2; i < new_size; ++i) {
        uint64_t p = spf[i];
        uint64_t m = i / p;
        if (m % p == 0) {                     /* repeated prime factor */
            rad_arr[i]   = rad_arr[m];
            omega_arr[i] = omega_arr[m];
        } else {
            rad_arr[i]   = rad_arr[m] * p;
            omega_arr[i] = omega_arr[m] + 1;
        }
    }

    cache_size = new_size;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n < 1; safe guard */

    ensure_cache(n);                /* guarantee rad_arr and omega_arr ready */

    uint64_t rad_n = rad_arr[n];
    uint64_t omega_n = omega_arr[n];

    seq_t count = 0;
    for (idx_t k = 2; k < n; ++k) {
        /* gcd(k,n) > 1 */
        if (gcd((seq_t)k, (seq_t)n) <= 1) continue;

        /* ω(k) > ω(n) */
        if ((seq_t)omega_arr[k] <= (seq_t)omega_n) continue;

        /* rad(n) | rad(k)  ->  rad(k) % rad(n) == 0 */
        if (rad_arr[k] % rad_n == 0) {
            ++count;
        }
    }
    return count;
}
