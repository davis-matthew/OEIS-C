/*
 * A295153  Numbers that have exactly five representations as a sum of five
 *          nonnegative squares.
 * https://oeis.org/A295153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumere les nombres ayant exactement cinq représentations
 * comme somme de cinq carrés non négatifs (sous-ensemble ordonné
 * de cinq entiers non négatifs, l'ordre étant ignoré). */

/* Sélections des constantes et types fournis par la construction
 * (ex. seq_t peut être uint8_t, uint16_t, uint32_t, uint64_t ou __int128,
 * idx_t est un entier sans signe de taille suffisante). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Valeur maximale à tester.  Au delà de 5408 les nombres ont
 * plus de cinq représentations, d'après la preuve d'A294675. */
#ifndef LIMIT
# define LIMIT 5408U
#endif

/* ---------- utilitaire ---------- */
/* racine carrée entière sans utilisation de doubles */
static inline idx_t int_sqrt_u64(idx_t n)
{
    if (n < 2)
        return n;
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* ---------- initialisation du tableau ---------- */
static seq_t *cached_values = NULL;
static idx_t cached_len = 0;
static int initialized = 0;

static void init_sequence(void)
{
    if (initialized)
        return;
    initialized = 1;

    /* tableau de compteurs de représentations (5100 valeurs suffisent) */
    uint16_t *cnt = (uint16_t *)calloc((size_t)(LIMIT + 1), sizeof(uint16_t));
    if (!cnt)
        return; /* échec d'allocation : rien à retourner */

    const idx_t LIM = int_sqrt_u64((idx_t)LIMIT);  /* ~ 73 */

    for (idx_t a = 0; a <= LIM; ++a) {
        idx_t a2 = a * a;
        if (a2 > LIMIT)
            break;
        for (idx_t b = a; b <= LIM; ++b) {
            idx_t b2 = b * b;
            idx_t s2 = a2 + b2;
            if (s2 > LIMIT)
                break;
            for (idx_t c = b; c <= LIM; ++c) {
                idx_t c2 = c * c;
                idx_t s3 = s2 + c2;
                if (s3 > LIMIT)
                    break;
                for (idx_t d = c; d <= LIM; ++d) {
                    idx_t d2 = d * d;
                    idx_t s4 = s3 + d2;
                    if (s4 > LIMIT)
                        break;
                    for (idx_t e = d; e <= LIM; ++e) {
                        idx_t e2 = e * e;
                        idx_t s5 = s4 + e2;
                        if (s5 > LIMIT)
                            break;
                        ++cnt[s5];
                    }
                }
            }
        }
    }

    /* Construction du tableau des valeurs avec exactement 5 représentations */
    size_t cap = 64;
    seq_t *list = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!list) {
        free(cnt);
        return; /* échec d'allocation */
    }
    size_t len = 0;
    for (idx_t n = 0; n <= LIMIT; ++n) {
        if (cnt[n] == 5u) {
            if (len == cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(list, cap * sizeof(seq_t));
                if (!tmp) {
                    free(list);
                    free(cnt);
                    return;
                }
                list = tmp;
            }
            list[len++] = (seq_t)n;
        }
    }

    free(cnt);
    cached_values = list;
    cached_len = len;
}

/* ---------- fonction publique ---------- */
seq_t a(idx_t n)
{
    init_sequence();

    if (n == 0)
        return (seq_t)0;           /* hors de l'offset */
    if (n > cached_len)
        return (seq_t)0;           /* hors de la suite (suite finie) */

    return cached_values[n - 1];
}
