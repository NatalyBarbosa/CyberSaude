import {
  StyleSheet,
  Switch,
  Text,
  Image,
  View,
  Alert,
} from 'react-native';

import { SafeAreaView } from 'react-native-safe-area-context';
import { useState } from 'react';

import ieee from '../../assets/images/ieee.jpeg';
import cyberSaude from '../../assets/images/cyber.jpeg';

export default function HomeScreen() {
  const [ligadoRed, setLigadoRed] = useState(false);
  const [ligadoYellow, setLigadoYellow] = useState(false);
  const [ligadoGreen, setLigadoGreen] = useState(false);

  async function controlarLed(led, estado) {
    const acao = estado ? 'ligar' : 'desligar';


    // PEGAR IP LA NO ARDUIDO IDE
    try {
      const resposta = await fetch(
        `http://IP DO WIFI/${led}/${acao}`
      );

      if (!resposta.ok) {
        throw new Error('Não foi possível controlar o LED');
      }

      return await resposta.text();
    } catch (erro) {
      console.error('Erro ao controlar LED:', erro);
      Alert.alert(
        'Erro de conexão',
        'Não foi possível se comunicar com o ESP32. Verifique a conexão Wi-Fi.'
      );

      return null;
    }
  }

  async function alterarLed(led, estado, atualizarEstado) {
    const resultado = await controlarLed(led, estado);

    if (resultado !== null) {
      atualizarEstado(estado);
    }
  }

  return (
    <SafeAreaView style={styles.container}>
      <Text style={styles.titulo}>IEEE Cyber Saúde</Text>

      <View style={styles.imagens}>
        <Image
          source={ieee}
          style={styles.image}
          resizeMode="contain"
        />

        <Image
          source={cyberSaude}
          style={styles.image}
          resizeMode="contain"
        />
      </View>

      <Text style={styles.subtitulo}>Controle dos LEDs</Text>

      {/* LED Vermelho */}
      <View style={styles.linha}>
        <View style={styles.identificacao}>
          <View
            style={[
              styles.indicador,
              { backgroundColor: '#E53935' },
            ]}
          />

          <Text style={styles.nomeLed}>LED Vermelho</Text>
        </View>

        <Switch
          value={ligadoRed}
          onValueChange={(estado) =>
            alterarLed('vermelho', estado, setLigadoRed)
          }
          trackColor={{
            true: '#E53935',
            false: '#BDBDBD',
          }}
          thumbColor="#FFFFFF"
        />
      </View>

      {/* LED Amarelo */}
      <View style={styles.linha}>
        <View style={styles.identificacao}>
          <View
            style={[
              styles.indicador,
              { backgroundColor: '#FBC02D' },
            ]}
          />

          <Text style={styles.nomeLed}>LED Amarelo</Text>
        </View>

        <Switch
          value={ligadoYellow}
          onValueChange={(estado) =>
            alterarLed('amarelo', estado, setLigadoYellow)
          }
          trackColor={{
            true: '#FBC02D',
            false: '#BDBDBD',
          }}
          thumbColor="#FFFFFF"
        />
      </View>

      {/* LED Verde */}
      <View style={styles.linha}>
        <View style={styles.identificacao}>
          <View
            style={[
              styles.indicador,
              { backgroundColor: '#2E7D32' },
            ]}
          />

          <Text style={styles.nomeLed}>LED Verde</Text>
        </View>

        <Switch
          value={ligadoGreen}
          onValueChange={(estado) =>
            alterarLed('verde', estado, setLigadoGreen)
          }
          trackColor={{
            true: '#2E7D32',
            false: '#BDBDBD',
          }}
          thumbColor="#FFFFFF"
        />
      </View>
    </SafeAreaView>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#F4F7FB',
    padding: 20,
  },

  titulo: {
    fontSize: 26,
    fontWeight: 'bold',
    textAlign: 'center',
    color: '#17365D',
    marginTop: 15,
    marginBottom: 25,
  },

  imagens: {
    flexDirection: 'row',
    justifyContent: 'space-evenly',
    alignItems: 'center',
    marginBottom: 30,
    gap: 12,
  },

  image: {
    width: '46%',
    height: 150,
  },

  subtitulo: {
    fontSize: 20,
    fontWeight: 'bold',
    color: '#17365D',
    marginBottom: 15,
  },

  linha: {
    flexDirection: 'row',
    alignItems: 'center',
    justifyContent: 'space-between',
    backgroundColor: '#FFFFFF',
    paddingHorizontal: 16,
    paddingVertical: 12,
    borderRadius: 12,
    marginBottom: 12,
    elevation: 2,
  },

  identificacao: {
    flexDirection: 'row',
    alignItems: 'center',
    gap: 12,
  },

  indicador: {
    width: 14,
    height: 14,
    borderRadius: 7,
  },

  nomeLed: {
    fontSize: 16,
    fontWeight: '500',
    color: '#333333',
  },
});