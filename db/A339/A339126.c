/*
 * A339126  Odd composite integers m such that A006497(m-J(m,13)) == 2*J(m,13)
 *          (mod m), where J(m,13) is the Jacobi symbol.
 * https://oeis.org/A339126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   grow_seq, is_prime_small, jacobi_symbol, mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A339126
   Definition: Odd composite integers m such that
   V(m - J(m,13)) ≡ 2·J(m,13) (mod m), where
   V(0)=2, V(1)=3, V(n+2)=3·V(n+1)+V(n) (the Pell‑Lucas
   sequence with parameters a=3,b=-1),
   J(m,13) is the Jacobi symbol.
   The sequence is indexed from a(1)=9. */

/*  The code builds the terms on demand and stores them in a
    simple dynamic array.  All arithmetic is performed on
    uint64_t.  The values are cast to seq_t only when stored in
    the array or returned.  This keeps the routine fast enough for
    n≤10000 and works for any seq_t that can hold the result. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/jacobi_symbol.h"
#include "../utils/is_prime_small.h"
#include "../utils/mod_mul.h"
#include "../utils/grow_seq.h"

/* Compute V(k) mod m for the Pell‑Lucas sequence with a=3,b=-1.
   m and k are 64‑bit unsigned integers.  All intermediate
   multiplications are performed via mod_mul to avoid overflow. */
static uint64_t lucas_mod_uint64(uint64_t m, uint64_t k)
{
    if (k == 0) return 2 % m;
    if (k == 1) return 3 % m;

    /* Matrix exponentiation:  |3 1|^k  applied to (V1,V0)ᵀ */
    uint64_t a = 1, b = 0, c = 0, d = 1;          /* identity */
    uint64_t BA = 3 % m, BB = 1 % m;
    uint64_t BC = 1 % m, BD = 0;

    while (k) {
        if (k & 1) {
            uint64_t na = (mod_mul(a, BA, m) + mod_mul(b, BC, m)) % m;
            uint64_t nb = (mod_mul(a, BB, m) + mod_mul(b, BD, m)) % m;
            uint64_t nc = (mod_mul(c, BA, m) + mod_mul(d, BC, m)) % m;
            uint64_t nd = (mod_mul(c, BB, m) + mod_mul(d, BD, m)) % m;
            a = na; b = nb; c = nc; d = nd;
        }
        /* square base matrix */
        uint64_t na = (mod_mul(BA, BA, m) + mod_mul(BB, BC, m)) % m;
        uint64_t nb = (mod_mul(BA, BB, m) + mod_mul(BB, BD, m)) % m;
        uint64_t nc = (mod_mul(BC, BA, m) + mod_mul(BD, BC, m)) % m;
        uint64_t nd = (mod_mul(BC, BB, m) + mod_mul(BD, BD, m)) % m;
        BA = na; BB = nb; BC = nc; BD = nd;
        k >>= 1;
    }
    /* V_k = c*V1 + d*V0 (mod m) */
    uint64_t res = (mod_mul(c, 3, m) + mod_mul(d, 2, m)) % m;
    return res;
}

/* Dynamic array to store found terms */
static seq_t *array = NULL;
static size_t capacity = 0;
static size_t length = 0;
static uint64_t last_checked = 7; /* value before first candidate (9) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    while (length < n) {
        uint64_t m = last_checked + 1;
        if ((m & 1) == 0) ++m;     /* first odd >= last_checked+1 */

        for (;;) {
            if (!is_prime_small(m)) {
                int j = jacobi_symbol(m, 13);          /* -1,0,1 */
                uint64_t k = m - (uint64_t)j;          /* safe cast */
                uint64_t vk = lucas_mod_uint64(m, k);

                uint64_t rhs;
                if (j == 0) {
                    rhs = 0;
                } else if (j == 1) {
                    rhs = 2 % m;
                } else { /* j == -1 */
                    rhs = (m - 2) % m;
                }

                if (vk == rhs) {
                    /* store the term */
                    grow_seq(&array, &capacity, length + 1);
                    array[length++] = (seq_t)m;
                    last_checked = m;
                    break;
                }
            }
            m += 2;
            if (m == 0) break; /* overflow guard, should not happen */
        }
    }
    return array[n - 1];
}
