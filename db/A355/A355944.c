/*
 * A355944  a(n) = smallest positive k such that n divides k*A276086(k), where
 *          A276086 is primorial base exp-function.
 * https://oeis.org/A355944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 9619, u32 n <= 9619, u64 n <= 9619,
 *            u128 n <= 9619, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A355944:
   a(n) = smallest positive k such that n divides k * A276086(k),
   where A276086(k) is the product of primes raised to the digits of k
   written in primorial base (weights = P[i] = product_{j=0}^{i-1} p_j,
   with P[0] = 1).  The implementation computes A276086(k) modulo n
   using the pre‑computed list of primes and primorials.                */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;          /* list of primes                 */
static size_t primes_cnt = 0;            /* number of primes in the list   */
static uint64_t *primorials = NULL;      /* primorials P[i]                */
static size_t primorial_cnt = 0;         /* number of primorials prepared  */
static idx_t max_k_for_arrays = 0;       /* largest k for which arrays are ready */

/* fast modular exponentiation: base^exp mod mod */
static seq_t mod_pow(seq_t base, seq_t exp, seq_t mod) {
    seq_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

/* Ensure we have primes and primorials up to at least max_k */
static void ensure_arrays(idx_t max_k) {
    if (max_k <= max_k_for_arrays) return;

    /* free old data if any */
    if (primes) free(primes);
    if (primorials) free(primorials);

    /* Get enough primes – a limit of 200 suffices for all max_k ≤ 1e4 */
    size_t limit = (max_k > 200) ? max_k : 200;
    build_primes_upto(limit, &primes, &primes_cnt);
    
    /* Build primorials P[0] = 1, P[i] = P[i-1] * primes[i-1] */
    primorial_cnt = 0;
    /* Allocate max possible size: at most primes_cnt primorials */
    primorials = (uint64_t*)malloc((primes_cnt+1) * sizeof(uint64_t));
    primorials[0] = 1;
    for (size_t i = 1; i <= primes_cnt; ++i) {
        uint64_t next = primorials[i-1] * primes[i-1];
        if (next > (uint64_t)max_k) break;
        primorials[i] = next;
        primorial_cnt = i;
    }
    max_k_for_arrays = max_k;
}

/* Compute A276086(k) modulo mod (mod == n) */
static seq_t A276086_mod(idx_t k, seq_t mod) {
    seq_t acc = 1 % mod;
    idx_t rem = k;
    for (idx_t i = primorial_cnt; i <= primorial_cnt; --i) { /* dummy to get unsigned loop start */
        if (i == 0) break; /* handle unsinged wrap */
    }
    for (idx_t i = primorial_cnt; ; --i) {
        seq_t d = rem / primorials[i];
        if (d) {
            seq_t p = (seq_t)(primes[i] % mod);
            seq_t term = mod_pow(p, d, mod);
            acc = (acc * term) % mod;
        }
        rem = rem % primorials[i];
        if (i == 0) break;
    }
    return acc;
}

seq_t a(idx_t n) {
    /*  Special case: n == 1 */
    if (n == 1) return (seq_t)1;

    /* Prepare helper tables up to n */
    ensure_arrays(n);

    seq_t mod = (seq_t)n;
    for (idx_t k = 1; k <= n; ++k) {
        /* Compute k * A276086(k) mod n */
        seq_t prod = A276086_mod(k, mod);
        seq_t total = ((seq_t)(k % n) * prod) % mod;
        if (total == 0) return (seq_t)k;
    }

    /* Should never reach here for the defined domain */
    return (seq_t)n;
}
